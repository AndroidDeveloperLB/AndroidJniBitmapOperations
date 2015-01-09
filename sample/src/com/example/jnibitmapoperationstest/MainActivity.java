
package com.example.jnibitmapoperationstest;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ImageView;
import com.jni.bitmap_operations.JniBitmapHolder;
import com.jni.bitmap_operations.JniBitmapHolder.ScaleMethod;

/** just a demo activity to show some of the features of the library */
@TargetApi(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
public class MainActivity extends Activity
{
    private static final int IMAGE_RESID_TO_TEST = R.drawable.test;
    JniBitmapHolder bitmapHolder = new JniBitmapHolder();

    @Override
    protected void onCreate(final Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //
        // original
        //
        final ImageView imageViewOriginal = (ImageView)findViewById(R.id.imageViewOriginal);
        final Bitmap b = BitmapFactory.decodeResource(getResources(), IMAGE_RESID_TO_TEST);
        imageViewOriginal.setImageBitmap(b);
        //
        // rotated 90 degrees CCW
        //
        final ImageView imageViewRotated90degreesCcw = (ImageView)findViewById(R.id.imageViewRotated90degreesCcw);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.rotateBitmapCcw90();
        imageViewRotated90degreesCcw.setImageBitmap(bitmapHolder.getBitmapAndFree());
        //
        // rotated 90 degrees CW
        //
        final ImageView imageViewRotated90degreesCw = (ImageView)findViewById(R.id.imageViewRotated90degreesCw);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.rotateBitmapCw90();
        imageViewRotated90degreesCw.setImageBitmap(bitmapHolder.getBitmapAndFree());
        //
        // rotate 180
        //
        final ImageView imageViewRotated180degreesCw = (ImageView)findViewById(R.id.imageViewRotated180degrees);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.rotateBitmap180();
        ;
        imageViewRotated180degreesCw.setImageBitmap(bitmapHolder.getBitmapAndFree());
        //
        // cropped
        //
        final ImageView imageViewCropped = (ImageView)findViewById(R.id.imageViewCropped);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.cropBitmap(b.getWidth() / 4, b.getHeight() / 4, b.getWidth() * 3 / 4,
                b.getHeight() * 3 / 4);
        imageViewCropped.setImageBitmap(bitmapHolder.getBitmapAndFree());
        //
        // scaled using nearest neighbor algorithm (which is fast, simple, yet
        // it sometimes has aliases problems)
        //
        final ImageView imageViewScaledUsingNearestNeighbour = (ImageView)findViewById(R.id.imageViewScaledUsingNearestNeighbour);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.scaleBitmap(b.getWidth() * 2, b.getHeight() * 2, ScaleMethod.NearestNeighbour);
        final Bitmap scaledBitmapNN = bitmapHolder.getBitmapAndFree();
        imageViewScaledUsingNearestNeighbour.setImageBitmap(scaledBitmapNN);
        //
        // scaled using nearest neighbor algorithm (which is relatively high
        // quality resizing and it handles aliases nicely)
        //
        final ImageView imageViewScaledUsingBilinearInterpolation = (ImageView)findViewById(R.id.imageViewScaledUsingBilinearInterpolation);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.scaleBitmap(b.getWidth() * 2, b.getHeight() * 2,
                ScaleMethod.BilinearInterpolation);
        final Bitmap scaledBitmapBI = bitmapHolder.getBitmapAndFree();
        imageViewScaledUsingBilinearInterpolation.setImageBitmap(scaledBitmapBI);
        //
        // flipped on the vertical
        //
        final ImageView imageViewFlippedVertical = (ImageView)findViewById(R.id.imageViewFlippedVertical);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.flipBitmapVertical();
        imageViewFlippedVertical.setImageBitmap(bitmapHolder.getBitmapAndFree());
        //
        // rotated 90 degrees CCW
        //
        final ImageView imageViewFlippedHorizontal = (ImageView)findViewById(R.id.imageViewFlippedHorizontal);
        bitmapHolder.storeBitmap(b);
        bitmapHolder.flipBitmapHorizontal();
        imageViewFlippedHorizontal.setImageBitmap(bitmapHolder.getBitmapAndFree());
    }

    @Override
    public boolean onCreateOptionsMenu(final Menu menu)
    {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(final MenuItem item)
    {
        String url = null;
        switch (item.getItemId())
        {
            case R.id.menuItem_all_my_apps:
                url = "https://play.google.com/store/apps/developer?id=AndroidDeveloperLB";
                break;
            case R.id.menuItem_all_my_repositories:
                url = "https://github.com/AndroidDeveloperLB";
                break;
            case R.id.menuItem_current_repository_website:
                url = "https://github.com/AndroidDeveloperLB/AndroidJniBitmapOperations";
                break;
        }
        if (url == null)
            return true;
        final Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(url));
        intent.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY
                | Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        startActivity(intent);
        return true;
    }
}
