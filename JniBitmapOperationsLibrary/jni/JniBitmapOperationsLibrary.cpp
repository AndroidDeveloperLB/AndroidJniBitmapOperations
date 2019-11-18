#include <jni.h>
#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <android/bitmap.h>
#include <cstring>
#include <unistd.h>

#define  LOG_TAG    "Applog"
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

//scale using Bilinear Interpolation
    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniScaleBIBitmap(
	    JNIEnv * env, jobject obj, jobject handle, uint32_t newWidth,
	    uint32_t newHeight);

    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFlipBitmapHorizontal(
	    JNIEnv * env, jobject obj, jobject handle);

    JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFlipBitmapVertical(
	    JNIEnv * env, jobject obj, jobject handle);
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

typedef struct
    {
    uint8_t alpha, red, green, blue;
    } ARGB;

int32_t convertArgbToInt(ARGB argb)
    {
    return (argb.alpha) | (argb.red << 24) | (argb.green << 16)
	    | (argb.blue << 8);
    }

void convertIntToArgb(uint32_t pixel, ARGB* argb)
    {
    argb->red = ((pixel >> 24) & 0xff);
    argb->green = ((pixel >> 16) & 0xff);
    argb->blue = ((pixel >> 8) & 0xff);
    argb->alpha = (pixel & 0xff);
    }

/**crops the bitmap within to be smaller. note that no validations are done*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniCropBitmap(
	JNIEnv * env, jobject obj, jobject handle, uint32_t left, uint32_t top,
	uint32_t right, uint32_t bottom)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
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
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
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
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
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
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* pixels = jniBitmap->_storedBitmapPixels;
    uint32_t* pixels2 = jniBitmap->_storedBitmapPixels;
    uint32_t width = jniBitmap->_bitmapInfo.width;
    uint32_t height = jniBitmap->_bitmapInfo.height;
    //no need to create a totally new bitmap - it's the exact same size as the original
    // 1234 fedc
    // 5678>ba09
    // 90ab>8765
    // cdef 4321
    int whereToGet = 0;
    for (int y = height - 1; y >= height / 2; --y)
	for (int x = width - 1; x >= 0; --x)
	    {
	    //take from each row (up to bottom), from left to right
	    uint32_t tempPixel = pixels2[width * y + x];
	    pixels2[width * y + x] = pixels[whereToGet];
	    pixels[whereToGet] = tempPixel;
	    ++whereToGet;
	    }
    //if the height isn't even, flip the middle row :
    if (height % 2 == 1)
	{
	int y = height / 2;
	whereToGet = width * y;
	int lastXToHandle = width % 2 == 0 ? (width / 2) : (width / 2) - 1;
	for (int x = width - 1; x >= lastXToHandle; --x)
	    {
	    uint32_t tempPixel = pixels2[width * y + x];
	    pixels2[width * y + x] = pixels[whereToGet];
	    pixels[whereToGet] = tempPixel;
	    ++whereToGet;
	    }
	}
    }

/**free bitmap*/ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFreeBitmapData(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
	return;
    delete[] jniBitmap->_storedBitmapPixels;
    jniBitmap->_storedBitmapPixels = NULL;
    delete jniBitmap;
    }

/**restore java bitmap (from JNI data)*/ //
JNIEXPORT jobject JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniGetBitmapFromStoredBitmapData(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
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

/**store java bitmap as JNI data*/ //
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
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
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

/**scales the image using a high-quality algorithm called "Bilinear Interpolation"
 * code is based on old university code I've made in Java: http://stackoverflow.com/questions/23230047/trying-to-convert-bilinear-interpolation-code-from-java-to-c-c-on-android/23302384#23302384
 * */ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniScaleBIBitmap(
	JNIEnv * env, jobject obj, jobject handle, uint32_t newWidth,
	uint32_t newHeight)
    {

    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t oldWidth = jniBitmap->_bitmapInfo.width;
    uint32_t oldHeight = jniBitmap->_bitmapInfo.height;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    uint32_t* newBitmapPixels = new uint32_t[newWidth * newHeight];
    // position of the top left pixel of the 4 pixels to use interpolation on
    int xTopLeft, yTopLeft;
    int x, y, lastTopLefty;
    float xRatio = (float) newWidth / (float) oldWidth, yratio =
	    (float) newHeight / (float) oldHeight;
    // Y color ratio to use on left and right pixels for interpolation
    float ycRatio2 = 0, ycRatio1 = 0;
    // pixel target in the src
    float xt, yt;
    // X color ratio to use on left and right pixels for interpolation
    float xcRatio2 = 0, xcratio1 = 0;
    ARGB rgbTopLeft, rgbTopRight, rgbBottomLeft, rgbBottomRight, rgbTopMiddle,
	    rgbBottomMiddle, result;
    for (x = 0; x < newWidth; ++x)
	{
	xTopLeft = (int) (xt = x / xRatio);
	// when meeting the most right edge, move left a little
	if (xTopLeft >= oldWidth - 1)
	    xTopLeft--;
	if (xt <= xTopLeft + 1)
	    {
	    // we are between the left and right pixel
	    xcratio1 = xt - xTopLeft;
	    // color ratio in favor of the right pixel color
	    xcRatio2 = 1 - xcratio1;
	    }
	for (y = 0, lastTopLefty = -30000; y < newHeight; ++y)
	    {
	    yTopLeft = (int) (yt = y / yratio);
	    // when meeting the most bottom edge, move up a little
	    if (yTopLeft >= oldHeight - 1)
		--yTopLeft;
	    if (lastTopLefty == yTopLeft - 1)
		{
		// we went down only one rectangle
		rgbTopLeft = rgbBottomLeft;
		rgbTopRight = rgbBottomRight;
		rgbTopMiddle = rgbBottomMiddle;
		//rgbBottomLeft=startingImageData[xTopLeft][yTopLeft+1];
		convertIntToArgb(
			previousData[((yTopLeft + 1) * oldWidth) + xTopLeft],
			&rgbBottomLeft);
		//rgbBottomRight=startingImageData[xTopLeft+1][yTopLeft+1];
		convertIntToArgb(
			previousData[((yTopLeft + 1) * oldWidth)
				+ (xTopLeft + 1)], &rgbBottomRight);
		rgbBottomMiddle.alpha = rgbBottomLeft.alpha * xcRatio2
			+ rgbBottomRight.alpha * xcratio1;
		rgbBottomMiddle.red = rgbBottomLeft.red * xcRatio2
			+ rgbBottomRight.red * xcratio1;
		rgbBottomMiddle.green = rgbBottomLeft.green * xcRatio2
			+ rgbBottomRight.green * xcratio1;
		rgbBottomMiddle.blue = rgbBottomLeft.blue * xcRatio2
			+ rgbBottomRight.blue * xcratio1;
		}
	    else if (lastTopLefty != yTopLeft)
		{
		// we went to a totally different rectangle (happens in every loop start,and might happen more when making the picture smaller)
		//rgbTopLeft=startingImageData[xTopLeft][yTopLeft];
		convertIntToArgb(previousData[(yTopLeft * oldWidth) + xTopLeft],
			&rgbTopLeft);
		//rgbTopRight=startingImageData[xTopLeft+1][yTopLeft];
		convertIntToArgb(
			previousData[(yTopLeft* oldWidth) + xTopLeft + 1],
			&rgbTopRight);
		rgbTopMiddle.alpha = rgbTopLeft.alpha * xcRatio2
			+ rgbTopRight.alpha * xcratio1;
		rgbTopMiddle.red = rgbTopLeft.red * xcRatio2
			+ rgbTopRight.red * xcratio1;
		rgbTopMiddle.green = rgbTopLeft.green * xcRatio2
			+ rgbTopRight.green * xcratio1;
		rgbTopMiddle.blue = rgbTopLeft.blue * xcRatio2
			+ rgbTopRight.blue * xcratio1;
		//rgbBottomLeft=startingImageData[xTopLeft][yTopLeft+1];
		convertIntToArgb(
			previousData[((yTopLeft + 1) * oldWidth) + xTopLeft],
			&rgbBottomLeft);
		//rgbBottomRight=startingImageData[xTopLeft+1][yTopLeft+1];
		convertIntToArgb(
			previousData[((yTopLeft + 1) * oldWidth)
				+ (xTopLeft + 1)], &rgbBottomRight);
		rgbBottomMiddle.alpha = rgbBottomLeft.alpha * xcRatio2
			+ rgbBottomRight.alpha * xcratio1;
		rgbBottomMiddle.red = rgbBottomLeft.red * xcRatio2
			+ rgbBottomRight.red * xcratio1;
		rgbBottomMiddle.green = rgbBottomLeft.green * xcRatio2
			+ rgbBottomRight.green * xcratio1;
		rgbBottomMiddle.blue = rgbBottomLeft.blue * xcRatio2
			+ rgbBottomRight.blue * xcratio1;
		}
	    lastTopLefty = yTopLeft;
	    if (yt <= yTopLeft + 1)
		{
		// color ratio in favor of the bottom pixel color
		ycRatio1 = yt - yTopLeft;
		ycRatio2 = 1 - ycRatio1;
		}
	    // prepared all pixels to look at, so finally set the new pixel data
	    result.alpha = rgbTopMiddle.alpha * ycRatio2
		    + rgbBottomMiddle.alpha * ycRatio1;
	    result.blue = rgbTopMiddle.blue * ycRatio2
		    + rgbBottomMiddle.blue * ycRatio1;
	    result.red = rgbTopMiddle.red * ycRatio2
		    + rgbBottomMiddle.red * ycRatio1;
	    result.green = rgbTopMiddle.green * ycRatio2
		    + rgbBottomMiddle.green * ycRatio1;
	    newBitmapPixels[(y * newWidth) + x] = convertArgbToInt(result);
	    }
	}
    //get rid of old data, and replace it with new one
    delete[] previousData;
    jniBitmap->_storedBitmapPixels = newBitmapPixels;
    jniBitmap->_bitmapInfo.width = newWidth;
    jniBitmap->_bitmapInfo.height = newHeight;
    }

/**flips a bitmap horizontally, as such:
 *
 * 123    321
 * 456 => 654
 * 789    987
 *
 * */ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFlipBitmapHorizontal(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    int width = jniBitmap->_bitmapInfo.width, middle = width / 2, height =
	    jniBitmap->_bitmapInfo.height;
    for (int y = 0; y < height; ++y)
	{
	//for each row, switch between the first pixels and the last ones
	uint32_t* idx1 = previousData + width * y;
	uint32_t* idx2 = previousData + width * (y + 1) - 1;
	for (int x = 0; x < middle; ++x)
	    {
	    uint32_t pixel = *idx1; //pixel= previousData[rowStart + x];
	    *idx1 = *idx2; //previousData[rowStart + x] =previousData[rowStart + (width - x - 1)];
	    *idx2 = pixel; //previousData[rowStart + (width - x - 1)] = pixel;
	    ++idx1;
	    --idx2;
	    }
	}
    }

/**flips a bitmap vertically, as such:
 *
 * 123    789
 * 456 => 456
 * 789    123
 *
 * */ //
JNIEXPORT void JNICALL Java_com_jni_bitmap_1operations_JniBitmapHolder_jniFlipBitmapVertical(
	JNIEnv * env, jobject obj, jobject handle)
    {
    JniBitmap* jniBitmap = (JniBitmap*) env->GetDirectBufferAddress(handle);
    if (jniBitmap == NULL || jniBitmap->_storedBitmapPixels == NULL)
	return;
    uint32_t* previousData = jniBitmap->_storedBitmapPixels;
    int width = jniBitmap->_bitmapInfo.width, height =
	    jniBitmap->_bitmapInfo.height, middle = height / 2;
    for (int y = 0; y < middle; ++y)
	{
	//for each row till the middle row, switch its pixels with the one at the bottom
	uint32_t* idx1 = previousData + width * y;
	uint32_t* idx2 = previousData + width * (height - y - 1);
	for (int x = 0; x < width; ++x)
	    {
	    uint32_t pixel =*idx1;
	    *idx1=*idx2;
	    *idx2=pixel;
	    ++idx2;
	    ++idx1;
	    }
	}
    }
