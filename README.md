AndroidJniBitmapOperations
==========================

Allows to perform various simple operations on bitmaps via JNI , while also providing some protection against OOM using the native Java environment on Android

This library is currently fully based on a post I've made on StackOverflow here:
http://stackoverflow.com/questions/18250951/jni-bitmap-operations-for-helping-to-avoid-oom-when-using-large-images/18250952?noredirect=1

Please read all of its notes.

How to import the library project
---------------------------------
Since ADT (at least till v22.6.2) still has problems importing Android libraries that have C/C++ code (made a post about it [**here**][1]) , the steps are:

 1. in case the library has a ".cproject" file , delete it. 
 2. in case the library has "cnature" or "ccnature" entries in the ".project" file, delete them, which look like:

 >  	  <nature>org.eclipse.cdt.core.cnature</nature>
 >  	  <nature>org.eclipse.cdt.core.ccnature</nature>

 3. delete folders "libs","gen","bin",obj" from the library folder. In case you have libraries, just remove the files you didn't add yourself.
 4. right click the library, choose "add native support..." via the "android tools" context menu. make sure the name of the suggested file is the same as your C/C++ file.
 5. build&compile the library
 6. you are ready to go.
 
For now, I've handled steps 1-3 , so all you need to do is the rest of the steps.


  [1]: http://stackoverflow.com/questions/22263253/how-to-correctly-import-an-android-library-with-jni-code/22956790?noredirect=1#comment35057887_22956790