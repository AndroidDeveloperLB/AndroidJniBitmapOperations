# AndroidJniBitmapOperations

Allows to perform various simple operations on bitmaps via JNI , while also providing some protection against OOM using the native Java environment on Android

Some of the operations are:
 - store/restore bitmaps to/from JNI.
 - rotate CW/CCW 90,180,270 degrees.
 - crop image.
 - flip image horizontally/vertically .
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

Starting from Android 11 (R - API 30), it seems to be possible to also decode the bitmaps right in JNI, so this might be handy to perform operations on it right away (though not sure how to do it) :
https://developer.android.com/ndk/guides/image-decoder

## Screenshot
Here's a sample of what can be done:

![animated demo](https://raw.githubusercontent.com/AndroidDeveloperLB/AndroidJniBitmapOperations/master/demo.gif)

## Known issues
Android-Studio still doesn't support C/C++ code well. It's easy to import the project and try it, but I think it's quite hard to do it for your own project.

## Missing features (TODO)

The things I think this library should have :

 1. using matrices for manipulating of the images.
 2. decode the image directly within JNI, instead of giving it from the Java "world". This should be very handy.
 3. use different bitmap formats. Also think how to manage them nicely.                                                            
 4. get current bitmap info.
 5. face detection
 6. rotation by any angle. 
 7. other basic operations that are available on the Android framework.
 8. Make more optimizations, perhaps by investigating the numebr of cache-misses, which is the biggest "enemy" for image manipulations in case of large bitmap. See [**this link**][3] for more information. 

## How to import the library project
### Eclipse

Since ADT (at least till v22.6.2) still has problems importing Android libraries that have C/C++ code (made a post about it [**here**][1]) , the steps are:

 1. in case the library has a ".cproject" file , delete it. 
 2. delete folders "libs","gen","bin",obj" from the library folder. In case you have libraries, just remove the files you didn't add yourself.
 3. in case the library has "cnature" or "ccnature" entries in the ".project" file, delete them, which look like:

 >     <nature>org.eclipse.cdt.core.cnature</nature>
 >     <nature>org.eclipse.cdt.core.ccnature</nature>
 >     <nature>org.eclipse.cdt.managedbuilder.core.managedBuildNature</nature>
>     <nature>org.eclipse.cdt.managedbuilder.core.ScannerConfigNature</nature>
  Also, you might need to delete those whole "buildCommand" tags (and their children) :

  >     org.eclipse.cdt.managedbuilder.core.genmakebuilder
  >     org.eclipse.cdt.managedbuilder.core.ScannerConfigBuilder

 4. right click the library, choose "add native support..." via the "android tools" context menu. make sure the name of the suggested file is the same as your C/C++ file. Make sure that it's being built using the [**NDK**][2] , or you won't be able to do it correctly.
 5. build&compile the library. 
 6. you are ready to go.


For now, I've handled steps 1-2 (I just made Git to ignore those files), so all you need to do is the rest of the steps.

### Android studio

Precondition: make sure that you have [**NDK**][2] installed and you either have this line in your `local.properties`

`ndk.dir=/path/to/ndk`

or you have `ANDROID_NDK_HOME` environment variable set.

#### Getting started

Just import the whole cloned project and run the sample.

#### Further configuring and using as the library

#### Option 1
 1. Add this repository as a git submodule. For these instructions we added in a folder named `AndroidJniBitmapOperations`
 2. Add the following lines to your `settings.gradle` file
 
    ```
    include ':JniBitmapOperationsLibrary'
    project(':JniBitmapOperationsLibrary').projectDir = new File(rootProject.getProjectDir(), 'AndroidJniBitmapOperations/JniBitmapOperationsLibrary')
    ```
 3. Add the following lines to your top level `build.gradle` file inside the `buildscript` section. Replace the versions with whatever your project is using as needed.
 
    ```
    // Variables for JniBitmapOperationsLibrary
    ext.propCompileSdkVersion = 23
    ext.propBuildToolsVersion = "27.0.3"
    ```
 4. Add the following lines to your app `build.gradle` file inside the `dependancies` section
 
    ```
    implementation project(':JniBitmapOperationsLibrary')
    ```

#### Option 2

 1. Copy `JniBitmapOperationsLibrary.cpp` into `src/main/jni` directory:
 
    ![Studio folder structure](https://s3.amazonaws.com/uploads.hipchat.com/22412/120721/qZyoFrgpUnFmnHu/upload.png)
 2. Add this minimum NDK config to your `build.gradle`
    
    ```
     android {
     ...
         defaultConfig {
         ...
             ndk {
                 moduleName "JniBitmapOperationsLibrary"
                 ldLibs "log",  "jnigraphics"
                 //optional: filter abis to compile for: abiFilters "x86", "armeabi-v7a"
                 //otherwise it will compile for all abis: "armeabi", "armeabi-v7a", "x86", and "mips"
             }
         }
     }
     ```
    
 3. Copy `JniBitmapHolder` into the project, putting it into the same package (`com.jni.bitmap_operations`).
 
You now should be able to use `JniBitmapHolder` to process images on NDK side.
 
##  Similar libraries

If you are interested in more features, and don't want to modify the code of this library, you could try out those similar libraries:

 - https://github.com/suckgamony/RapidDecoder
 - https://github.com/facebook/fresco
 - https://android-arsenal.com/tag/63


[1]: http://stackoverflow.com/questions/22263253/how-to-correctly-import-an-android-library-with-jni-code/22956790?noredirect=1#comment35057887_22956790

 [2]: https://developer.android.com/tools/sdk/ndk/index.html
 
 [3]: http://www.powershow.com/view/29fcd-NjRmN/Fast_matrix_multiplication_Cache_usage_powerpoint_ppt_presentation
