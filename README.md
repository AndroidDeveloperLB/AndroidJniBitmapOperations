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

 1. using matrices for manipulating of the imaages.
 2. decode the image directly within JNI, instead of giving it from the Java "world". This should be very handy.
 3. use different bitmap formats. Also think how to manage them nicely.                                                            
 4. get current bitmap info.
 5. face detection
 6. rotation by any angle. 
 7. other basic operations that are available on the Android framework.

How to import the library project
---------------------------------
Since ADT (at least till v22.6.2) still has problems importing Android libraries that have C/C++ code (made a post about it [**here**][1]) , the steps are:

 1. in case the library has a ".cproject" file , delete it. 
 2. delete folders "libs","gen","bin",obj" from the library folder. In case you have libraries, just remove the files you didn't add yourself.
 3. in case the library has "cnature" or "ccnature" entries in the ".project" file, delete them, which look like:

 >  	  <nature>org.eclipse.cdt.core.cnature</nature>
 >  	  <nature>org.eclipse.cdt.core.ccnature</nature>
   
 4. right click the library, choose "add native support..." via the "android tools" context menu. make sure the name of the suggested file is the same as your C/C++ file.
 5. build&compile the library
 6. you are ready to go.


For now, I've handled steps 1-2 (I just made Git to ignore those files), so all you need to do is the rest of the steps.


 [1]: http://stackoverflow.com/questions/22263253/how-to-correctly-import-an-android-library-with-jni-code/22956790?noredirect=1#comment35057887_22956790
