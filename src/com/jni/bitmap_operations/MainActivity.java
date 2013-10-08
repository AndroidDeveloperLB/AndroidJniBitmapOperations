package com.jni.bitmap_operations;
import android.annotation.TargetApi;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.widget.ImageView;

/** just a demo activity to show some of the features of the library */
@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
public class MainActivity extends Activity
  {
  JniBitmapHolder bitmapHolder =new JniBitmapHolder();

  @Override
  protected void onCreate(final Bundle savedInstanceState)
    {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    //
    // original
    //
    final ImageView imageViewOriginal=(ImageView)findViewById(R.id.imageViewOriginal);
    final Bitmap b=BitmapFactory.decodeResource(getResources(),android.R.drawable.sym_def_app_icon);
    imageViewOriginal.setImageBitmap(b);
    //
    // rotated 90 degress CCW
    //
    final ImageView imageViewRotated90degressCcw=(ImageView)findViewById(R.id.imageViewRotated90degressCcw);
    bitmapHolder.storeBitmap(b);
    bitmapHolder.rotateBitmapCcw90();
    imageViewRotated90degressCcw.setImageBitmap(bitmapHolder.getBitmapAndFree());
    //
    // cropped
    //
    final ImageView imageViewCropped=(ImageView)findViewById(R.id.imageViewCropped);
    bitmapHolder.storeBitmap(b);
    bitmapHolder.cropBitmap(b.getWidth()/4,b.getHeight()/4,b.getWidth()*3/4,b.getHeight()*3/4);
    imageViewCropped.setImageBitmap(bitmapHolder.getBitmapAndFree());
    //
    }
  }
