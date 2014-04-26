package com.example.jnibitmapoperationstest;
import android.graphics.Bitmap;
import android.graphics.Color;

/** class for resizing imageData using the Bilinear Interpolation method */
public class BilinearInterpolation
  {
  /** the method for resizing the imageData using the Bilinear Interpolation algorithm */
  public static void resize(final Bitmap input,final Bitmap output)
    {
    final int oldHeight=input.getHeight(),oldWidth=input.getWidth();
    final int newHeight=output.getHeight(),newWidth=output.getWidth();
    // position of the top left pixel of the 4 pixels to use interpolation on
    int xTopLeft,yTopLeft;
    int x,y,lastTopLefty;
    final float xRatio=(float)newWidth/(float)oldWidth,yratio=(float)newHeight/(float)oldHeight;
    // Y color ratio to use on left and right pixels for interpolation
    float ycRatio2=0,ycRatio1=0;
    // pixel target in the src
    float xt,yt;
    // X color ratio to use on left and right pixels for interpolation
    float xcRatio2=0,xcratio1=0;
    int rgbTopLeft=0,rgbTopRight=0,rgbBottomLeft=0,rgbBottomRight=0,rgbTopMiddle=0,rgbBottomMiddle=0;
    // do the resizing:
    for(x=0;x<newWidth;x++)
      {
      xTopLeft=(int)(xt=x/xRatio);
      // when meeting the most right edge, move left a little
      if(xTopLeft>=oldWidth-1)
        xTopLeft--;
      if(xt<=xTopLeft+1)
        {
        // we are between the left and right pixel
        xcratio1=xt-xTopLeft;
        // color ratio in favor of the right pixel color
        xcRatio2=1-xcratio1;
        }
      for(y=0,lastTopLefty=Integer.MIN_VALUE;y<newHeight;y++)
        {
        yTopLeft=(int)(yt=y/yratio);
        // when meeting the most bottom edge, move up a little
        if(yTopLeft>=oldHeight-1)
          yTopLeft--;
        // we went down only one rectangle
        if(lastTopLefty==yTopLeft-1)
          {
          rgbTopLeft=rgbBottomLeft;
          rgbTopRight=rgbBottomRight;
          rgbTopMiddle=rgbBottomMiddle;
          rgbBottomLeft=input.getPixel(xTopLeft,yTopLeft+1);
          rgbBottomRight=input.getPixel(xTopLeft+1,yTopLeft+1);
          rgbBottomMiddle=Color.argb((int)(Color.alpha(rgbBottomLeft)*xcRatio2+Color.alpha(rgbBottomRight)*xcratio1),//
              (int)(Color.red(rgbBottomLeft)*xcRatio2+Color.red(rgbBottomRight)*xcratio1),//
              (int)(Color.green(rgbBottomLeft)*xcRatio2+Color.green(rgbBottomRight)*xcratio1),//
              (int)(Color.blue(rgbBottomLeft)*xcRatio2+Color.blue(rgbBottomRight)*xcratio1));
          }
        else if(lastTopLefty!=yTopLeft)
          {
          // we went to a totally different rectangle (happens in every loop start,and might happen more when making the picture smaller)
          rgbTopLeft=input.getPixel(xTopLeft,yTopLeft);
          rgbTopRight=input.getPixel(xTopLeft+1,yTopLeft);
          rgbTopMiddle=Color.argb((int)(Color.alpha(rgbTopLeft)*xcRatio2+Color.alpha(rgbTopRight)*xcratio1),//
              (int)(Color.red(rgbTopLeft)*xcRatio2+Color.red(rgbTopRight)*xcratio1),//
              (int)(Color.green(rgbTopLeft)*xcRatio2+Color.green(rgbTopRight)*xcratio1),//
              (int)(Color.blue(rgbTopLeft)*xcRatio2+Color.blue(rgbTopRight)*xcratio1));
          rgbBottomLeft=input.getPixel(xTopLeft,yTopLeft+1);
          rgbBottomRight=input.getPixel(xTopLeft+1,yTopLeft+1);
          rgbBottomMiddle=Color.argb((int)(Color.alpha(rgbBottomLeft)*xcRatio2+Color.alpha(rgbBottomRight)*xcratio1),//
              (int)(Color.red(rgbBottomLeft)*xcRatio2+Color.red(rgbBottomRight)*xcratio1),//
              (int)(Color.green(rgbBottomLeft)*xcRatio2+Color.green(rgbBottomRight)*xcratio1),//
              (int)(Color.blue(rgbBottomLeft)*xcRatio2+Color.blue(rgbBottomRight)*xcratio1));
          }
        lastTopLefty=yTopLeft;
        if(yt<=yTopLeft+1)
          {
          // color ratio in favor of the bottom pixel color
          ycRatio1=yt-yTopLeft;
          ycRatio2=1-ycRatio1;
          }
        // prepared all pixels to look at, so finally set the new pixel data
        output.setPixel(x,y,Color.argb(//
            (int)(Color.alpha(rgbTopMiddle)*ycRatio2+Color.alpha(rgbBottomMiddle)*ycRatio1),//
            (int)(Color.red(rgbTopMiddle)*ycRatio2+Color.red(rgbBottomMiddle)*ycRatio1),//
            (int)(Color.green(rgbTopMiddle)*ycRatio2+Color.green(rgbBottomMiddle)*ycRatio1),//
            (int)(Color.blue(rgbTopMiddle)*ycRatio2+Color.blue(rgbBottomRight)*ycRatio1)));
        }
      }
    }
  }
