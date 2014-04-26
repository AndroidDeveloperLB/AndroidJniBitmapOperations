LOCAL_PATH := $(call my-dir)

#bitmap operations module
include $(CLEAR_VARS)

LOCAL_MODULE    := JniBitmapOperationsLibrary
LOCAL_SRC_FILES := JniBitmapOperationsLibrary.cpp
LOCAL_LDLIBS := -llog
LOCAL_LDFLAGS += -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
APP_OPTIM := debug
LOCAL_CFLAGS := -g

#if you need to add more module, do the same as the one we started with (the one with the CLEAR_VARS)
