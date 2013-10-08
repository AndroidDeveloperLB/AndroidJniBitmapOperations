package com.jni.bitmap_operations;
import android.graphics.Bitmap;

public class JniBitmapRotation
  {
  static
    {
    System.loadLibrary("JniBitmapRotationTest");
    }

  /**
   * rotates a bitmap by 90 degrees counter-clockwise . <br/>
   * notes:<br/>
   * -the input bitmap will be recycled and shouldn't be used anymore <br/>
   * -returns the rotated bitmap . <br/>
   * -could take some time , so do the operation in a new thread
   */
  public static native Bitmap rotateBitmapCcw90(Bitmap bitmap);
  }
