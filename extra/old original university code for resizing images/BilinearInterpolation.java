package ex1;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.graphics.RGB;

/** class for resizing imageData using the Bilinear Interpolation method */
public class BilinearInterpolation
  {
  static ImageData lastImageData =null; // the last imageData that we used. changes only when we handle a new image, so it stays the same as long as we handle the same image
  static RGB[][]   startingImageData;

  /** the method for resizing the imageData using the Bilinear Interpolation algorithm */
  public static void resize(ImageData inputImageData,ImageData newImageData,int oldWidth,int oldHeight,int newWidth,int newHeight)
    {
    boolean gotNewImage=inputImageData!=lastImageData;
    lastImageData=inputImageData;
    int xTopLeft,yTopLeft; // position of the top left pixel of the 4 pixels to use interpolation on
    int x,y,lastTopLefty;
    float xRatio=(float)newWidth/(float)oldWidth,yratio=(float)newHeight/(float)oldHeight;
    float ycRatio2=0,ycRatio1=0; // Y color ratio to use on left and right pixels for interpolation
    float xt,yt; // pixel target in the src
    float xcRatio2=0,xcratio1=0; // X color ratio to use on left and right pixels for interpolation
    RGB rgbTopLeft,rgbTopRight,rgbBottomLeft=null,rgbBottomRight=null,rgbTopMiddle=null,rgbBottomMiddle=null;
    if(gotNewImage)
      {
      startingImageData=new RGB[oldWidth][oldHeight];
      for(x=0;x<oldWidth;++x)
        for(y=0;y<oldHeight;++y)
          {
          rgbTopLeft=inputImageData.palette.getRGB(inputImageData.getPixel(x,y));
          startingImageData[x][y]=new RGB(rgbTopLeft.red,rgbTopLeft.green,rgbTopLeft.blue);
          }
      }
    for(x=0;x<newWidth;x++)
      {
      xTopLeft=(int)(xt=x/xRatio);
      if(xTopLeft>=oldWidth-1) xTopLeft--;// when meeting the most right edge, move left a little
      if(xt<=xTopLeft+1)// we are between the left and right pixel
        {
        xcratio1=xt-xTopLeft;// color ratio in favor of the right pixel color
        xcRatio2=1-xcratio1;
        }
      for(y=0,lastTopLefty=Integer.MIN_VALUE;y<newHeight;y++)
        {
        yTopLeft=(int)(yt=y/yratio);
        if(yTopLeft>=oldHeight-1) yTopLeft--;// when meeting the most bottom edge, move up a little
        if(lastTopLefty==yTopLeft-1)// we went down only one rectangle
          {
          rgbTopLeft=rgbBottomLeft;
          rgbTopRight=rgbBottomRight;
          rgbTopMiddle=rgbBottomMiddle;
          rgbBottomLeft=startingImageData[xTopLeft][yTopLeft+1];
          rgbBottomRight=startingImageData[xTopLeft+1][yTopLeft+1];
          rgbBottomMiddle=new RGB((int)(rgbBottomLeft.red*xcRatio2+rgbBottomRight.red*xcratio1),(int)(rgbBottomLeft.green*xcRatio2+rgbBottomRight.green*xcratio1),(int)(rgbBottomLeft.blue*xcRatio2+rgbBottomRight.blue*xcratio1));
          }
        else if(lastTopLefty!=yTopLeft)
          { // we went to a totally different rectangle (happens in every loop start,and might happen more when making the picture smaller)
          rgbTopLeft=startingImageData[xTopLeft][yTopLeft];
          rgbTopRight=startingImageData[xTopLeft+1][yTopLeft];
          rgbTopMiddle=new RGB((int)(rgbTopLeft.red*xcRatio2+rgbTopRight.red*xcratio1),(int)(rgbTopLeft.green*xcRatio2+rgbTopRight.green*xcratio1),(int)(rgbTopLeft.blue*xcRatio2+rgbTopRight.blue*xcratio1));
          rgbBottomLeft=startingImageData[xTopLeft][yTopLeft+1];
          rgbBottomRight=startingImageData[xTopLeft+1][yTopLeft+1];
          rgbBottomMiddle=new RGB((int)(rgbBottomLeft.red*xcRatio2+rgbBottomRight.red*xcratio1),(int)(rgbBottomLeft.green*xcRatio2+rgbBottomRight.green*xcratio1),(int)(rgbBottomLeft.blue*xcRatio2+rgbBottomRight.blue*xcratio1));
          }
        lastTopLefty=yTopLeft;
        if(yt<=yTopLeft+1)
          {
          ycRatio1=yt-yTopLeft;// color ratio in favor of the bottom pixel color
          ycRatio2=1-ycRatio1;
          }
        newImageData.setPixel(x,y,inputImageData.palette.getPixel(new RGB((int)(rgbTopMiddle.red*ycRatio2+rgbBottomMiddle.red*ycRatio1),(int)(rgbTopMiddle.green*ycRatio2+rgbBottomMiddle.green*ycRatio1),(int)(rgbTopMiddle.blue*ycRatio2+rgbBottomMiddle.blue*ycRatio1))));
        }
      }
    }
  }
