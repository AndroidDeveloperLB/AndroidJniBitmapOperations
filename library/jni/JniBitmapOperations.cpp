#include <jni.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>

#define  LOG_TAG    "DEBUG"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
    {
    //store
    JNIEXPORT jobject JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniStoreBitmapData(
	    JNIEnv * env, jobject obj, jobject bitmap);
    //get
    JNIEXPORT jobject JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniGetBitmapFromStoredBitmapData(
	    JNIEnv * env, jobject obj, jobject handle);
    //free
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFreeBitmapData(
	    JNIEnv * env, jobject obj, jobject handle);
    //rotate 90 degrees CCW
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmapCcw90(
	    JNIEnv * env, jobject obj, jobject handle);
    //rotate 90 degrees CW
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmapCw90(
	    JNIEnv * env, jobject obj, jobject handle);
    //rotate 180 degrees
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmap180(
	    JNIEnv * env, jobject obj, jobject handle);
    //crop
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniCropBitmap(
	    JNIEnv * env, jobject obj, jobject handle, uint32_t left,
	    uint32_t top, uint32_t right, uint32_t bottom);
    //scale using nearest neighbor
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniScaleNNBitmap(
	    JNIEnv * env, jobject obj, jobject handle, uint32_t newWidth,
	    uint32_t newHeight);
    }

class JniBitmap
    {
public:
    uint32_t* _storedBitmapPixels;
    AndroidBitmapInfo _bitmapInfo;
    JniBitmap()
	{
	_storedBitmapPixels = NULL;
	}
    };

/**crops the bitmap within to be smaller. note that no validations are done*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniCropBitmap(
	JNIEnv * env, jobject obj, jobject handle, uint32_t left, uint32_t top,
	uint32_t right, uint32_t bottom)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t oldWidth = jniBitmap->_bitmapInfo.width;
    uint32_t newWidth = right - left, newHeight = bottom - top;
    uint32_t* newBitmapPixels = new uint32_t[newWidth * newHeight];
    uint32_t* whereToGet = previousData + left + top * oldWidth;
    uint32_t* whereToPut = newBitmapPixels;
    for (int y = top; y < bottom; ++y)
	{
	memcpy(whereToPut, whereToGet, sizeof(uint32_t) * newWidth);
	whereToGet += oldWidth;
	whereToPut += newWidth;
	}
    //done copying , so replace old data with new one
    delete[] previousData;
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    jniBitmap->_bitmapInfo.width = newWidth;
    jniBitmap->_bitmapInfo.height = newHeight;
    }

/**rotates the inner bitmap data by 90 degrees counter clock wise*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmapCcw90(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t newWidth = jniBitmap->_bitmapInfo.height;
    uint32_t newHeight = jniBitmap->_bitmapInfo.width;
    jniBitmap->_bitmapInfo.width = newWidth;
    jniBitmap->_bitmapInfo.height = newHeight;
    uint32_t* newBitmapPixels = new uint32_t[newWidth * newHeight];
    int whereToGet = 0;
    // XY. ... ... ..X
    // ...>Y..>...>..Y
    // ... X.. .YX ...
    for (int x = 0; x < newWidth; ++x)
	for (int y = newHeight - 1; y >= 0; --y)
	    {
	    //take from each row (up to bottom), from left to right
	    uint32_t pixel = previousData[whereToGet++];
	    newBitmapPixels[newWidth * y + x] = pixel;
	    }
    delete[] previousData;
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    }

/**rotates the inner bitmap data by 90 degrees clock wise*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmapCw90(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t newWidth = jniBitmap->_bitmapInfo.height;
    uint32_t newHeight = jniBitmap->_bitmapInfo.width;
    jniBitmap->_bitmapInfo.width = newWidth;
    jniBitmap->_bitmapInfo.height = newHeight;
    uint32_t* newBitmapPixels = new uint32_t[newWidth * newHeight];
    int whereToGet = 0;
    // XY. ..X ... ...
    // ...>..Y>...>Y..
    // ... ... .YX X..
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    for (int x = newWidth - 1; x >= 0; --x)
	for (int y = 0; y < newHeight; ++y)
	    {
	    //take from each row (up to bottom), from left to right
	    uint32_t pixel = previousData[whereToGet++];
	    newBitmapPixels[newWidth * y + x] = pixel;
	    }
    delete[] previousData;
    }

/**rotates the inner bitmap data by 180 degrees (*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniRotateBitmap180(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t width = jniBitmap->_bitmapInfo.width;
    uint32_t height = jniBitmap->_bitmapInfo.height;
    uint32_t* newBitmapPixels = new uint32_t[width * height];
    int whereToGet = 0;
    // XY. ...
    // ...>...
    // ... .YX
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    for (int y = height - 1; y >= 0; --y)
	for (int x = width - 1; x >= 0; --x)
	    {
	    //take from each row (up to bottom), from left to right
	    uint32_t pixel = previousData[whereToGet++];
	    newBitmapPixels[width * y + x] = pixel;
	    }
    delete[] previousData;
    }

/**free bitmap*/  //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFreeBitmapData(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    delete[] jniBitmap->_storedBitmapPixels;
    jniBitmap->_storedBitmapPixels = NULL;
    delete jniBitmap;
    }

/**restore java bitmap (from JNI data)*/  //
JNIEXPORT jobject JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniGetBitmapFromStoredBitmapData(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	{
	LOGD("no bitmap data was stored. returning null...");
	return NULL;
	}
    //
    //creating a new bitmap to put the pixels into it - using Bitmap Bitmap.createBitmap (int width, int height, Bitmap.Config config) :
    //
    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
	    "createBitmap",
	    "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(
	    bitmapConfigClass, "valueOf",
	    "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
	    valueOfBitmapConfigFunction, configName);
    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
	    createBitmapFunction, jniBitmap->_bitmapInfo.width,
	    jniBitmap->_bitmapInfo.height, bitmapConfig);
    //
    // putting the pixels into the new bitmap:
    //
    int ret;
    void* bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, newBitmap, &bitmapPixels)) < 0)
	{
	LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	return NULL;
	}
    uint32_t* newBitmapPixels = (uint32_t*) bitmapPixels;
    int pixelsCount = jniBitmap->_bitmapInfo.height
	    * jniBitmap->_bitmapInfo.width;
    memcpy(newBitmapPixels, jniBitmap->_storedBitmapPixels,
	    sizeof(uint32_t) * pixelsCount);
    AndroidBitmap_unlockPixels(env, newBitmap);
    //LOGD("returning the new bitmap");
    return newBitmap;
    }

/**store java bitmap as JNI data*/  //
JNIEXPORT jobject JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniStoreBitmapData(
	JNIEnv * env, jobject obj, jobject bitmap)
    {
    AndroidBitmapInfo bitmapInfo;
    uint32_t* storedBitmapPixels = NULL;
    //LOGD("reading bitmap info...");
    int ret;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0)
	{
	LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
	return NULL;
	}
    //LOGD("width:%d height:%d stride:%d", bitmapInfo.width, bitmapInfo.height, bitmapInfo.stride);
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
	LOGE("Bitmap format is not RGBA_8888!");
	return NULL;
	}
    //
    //read pixels of bitmap into native memory :
    //
    //LOGD("reading bitmap pixels...");
    void* bitmapPixels;
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0)
	{
	LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	return NULL;
	}
    uint32_t* src = (uint32_t*) bitmapPixels;
    storedBitmapPixels = new uint32_t[bitmapInfo.height * bitmapInfo.width];
    int pixelsCount = bitmapInfo.height * bitmapInfo.width;
    memcpy(storedBitmapPixels, src, sizeof(uint32_t) * pixelsCount);
    AndroidBitmap_unlockPixels(env, bitmap);
    JniBitmap *jniBitmap = new JniBitmap();
    jniBitmap->_bitmapInfo = bitmapInfo;
    jniBitmap->_storedBitmapPixels = storedBitmapPixels;
    return env->NewDirectByteBuffer(jniBitmap, 0);
    }

/**scales the image using the fastest, simplest algorithm called "nearest neighbor" */ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniScaleNNBitmap(
	JNIEnv * env, jobject obj, jobject handle, uint32_t newWidth,
	uint32_t newHeight)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t oldWidth = jniBitmap->_bitmapInfo.width;
    uint32_t oldHeight = jniBitmap->_bitmapInfo.height;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t* newBitmapPixels = new uint32_t[newWidth * newHeight];
    int x2, y2;
    int whereToPut = 0;
    for (int y = 0; y < newHeight; ++y)
	{
	for (int x = 0; x < newWidth; ++x)
	    {
	    x2 = x * oldWidth / newWidth;
	    if (x2 < 0)
		x2 = 0;
	    else if (x2 >= oldWidth)
		x2 = oldWidth - 1;
	    y2 = y * oldHeight / newHeight;
	    if (y2 < 0)
		y2 = 0;
	    else if (y2 >= oldHeight)
		y2 = oldHeight - 1;
	    newBitmapPixels[whereToPut++] = previousData[(y2 * oldWidth) + x2];
	    //same as : newBitmapPixels[(y * newWidth) + x] = previousData[(y2 * oldWidth) + x2];
	    }
	}

    delete[] previousData;
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    jniBitmap->_bitmapInfo.width = newWidth;
    jniBitmap->_bitmapInfo.height = newHeight;
    }

