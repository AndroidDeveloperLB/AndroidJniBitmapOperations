package ex1;
import org.eclipse.swt.graphics.ImageData;

public class NearestNeighbor
  {
  public static void resize(ImageData originalImageData,ImageData newImageData,int oldwidth,int oldheight,int newWidth,int newHeight)
    {
    float x,y,xratio=(float)newWidth/(float)oldwidth,yratio=(float)newHeight/(float)oldheight;
    for(x=0.0f;x<newWidth;x++)
      for(y=0.0f;y<newHeight;y++)
        newImageData.setPixel((int)x,(int)y,originalImageData.getPixel((int)(x/xratio),(int)(y/yratio)));
    }
  }
