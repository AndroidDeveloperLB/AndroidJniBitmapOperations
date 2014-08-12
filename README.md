AndroidJniBitmapOperations
==========================

Allows to perform various simple operations on bitmaps via JNI , while also providing some protection against OOM using the native Java environment on Android

Some of the operations are:
 - store/restore bitmaps to/from JNI.
 - rotate CW/CCW 90,180,270 degrees.
 - crop image
 - scale image using either "Nearest-Neighbor" algorithm or "Bilinear-Interpolation" algorithm.
 The first is fast but might cause aliasing artifacts on some cases, and the other is a bit slower but resizes the images nicely and avoids having aliasing artifacts. 
 However, it cause the output image to be a bit softer/blurry. 
 More information about those algorithms here:
 http://en.wikipedia.org/wiki/Image_scaling
 
As the resizing algorithms deal with colors, they also show how to create your own algorithms for handling pixels. 
You can make filters and implement other ways to resize images. Please consider contributing your own code for such operations.

This library was first introduced via StackOverflow, and many of the notes written there still hold now.
Please read it here:
http://stackoverflow.com/questions/18250951/jni-bitmap-operations-for-helping-to-avoid-oom-when-using-large-images/18250952?noredirect=1

Missing features (TODO)
-----------------------

The things I think this library should have :

 1. using matrices for manipulating of the images.
 2. decode the image directly within JNI, instead of giving it from the Java "world". This should be very handy.
 3. use different bitmap formats. Also think how to manage them nicely.                                                            
 4. get current bitmap info.
 5. face detection
 6. rotation by any angle. 
 7. other basic operations that are available on the Android framework.

How to import the library project
---------------------------------

 1. Create a folder in your project and call it "libs" (if you dont have one already), create another folder inside the "libs" folder you just created and call it "armeabi".
 2. place the [**JniBitmapOperationLibrary.so**][1] into the "armeabi" folder.
 3. place the [**JniBitmapHolder.java**][2] in your "src" folder.
 4. Thats all :)


 [1]:  http://bit.ly/JniBitmapSO
 
 [2]:  https://github.com/AndroidDeveloperLB/AndroidJniBitmapOperations/tree/master/JniBitmapOperationsLibrary/src/com/jni/bitmap_operations
 
 Usage:
-----------------------

```
// The Bitmap we want to hold in JNI
Bitmap bitmap = BitmapFactory.decodeResource(getResources(), R.drawable.large_wallpaper);
   	
//hold the bitmap in JNI (this will also release the bitmap in the java "world")
JniBitmapHolder bitmapHolder = new JniBitmapHolder(bitmap);
				
//get the bitmap and free the memory
mImgeView.setImageBitmap(bitmapHolder.getBitmapAndFree());
				
```
This is just a simple overview. look at the example project for a more detailed overview. Enjoy :)
