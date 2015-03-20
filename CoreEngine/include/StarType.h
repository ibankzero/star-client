//
//  iOSType.h
//  StarEngine
//
//  Created by Samret Wajanasathian on 4/9/2554.
//  Copyright (c) 2554 __MyCompanyName__. All rights reserved.
//

#ifndef StarEngine_StarSType_h
#define StarEngine_StarSType_h

//#define DT_DEBUG

#ifdef __cplusplus
    #define EXTERN_C       extern "C"
	#define EXTERN_C_BEGIN extern "C" {
	#define EXTERN_C_END   }
#else
	#define EXTERN_C       /* Nothing */
	#define EXTERN_C_BEGIN /* Npthing */
	#define EXTERN_C_END   /* Nothing */
#endif


#if defined(DEBUG) || defined(_DEBUG)
  #define STAR_DEBUG 1
#endif

#ifdef IOS_VERSION
    #define STAR_IOS_VERSION          0
    #include <MacTypes.h>
#elif MAC_OS_VERSION
    #define STAR_OSX_VERSION          3
    #include <MacTypes.h>
#elif ANDROID_VERSION
    #define STAR_ANDROID_VERSION      1
#elif WIN32
	#define STAR_WINDOWS_VERSION     3
	#pragma warning disable
#elif WIN64
	#define STAR_WINDOWS_VERSION     3
	#pragma warning disable
#endif

#ifdef STAR_WINDOWS_VERSION
	#define WIN_DLL __declspec(dllexport)
#else
	#define WIN_DLL 
#endif

#include "stdio.h"

#define PANE_NAME_SIZE  32
#define EXPORT_VERSION  15
#define EXPORT_VERSION_64BIT  16
#define FONT_VERSION 4
#define FONT_VERSION_64BIT 5

#ifdef __LP64__
    // 64-bit code
    typedef unsigned char   u8;
    typedef char            s8;
    typedef unsigned short  u16;
    typedef short           s16;
    typedef unsigned int    u32;
    typedef int             s32;
    typedef float           f32;
    typedef double          f64;
#else
    // 32-bit code
    typedef unsigned char   u8;
    typedef char            s8;
    typedef unsigned short  u16;
    typedef short           s16;
    typedef unsigned int    u32;
    typedef int             s32;
    typedef float           f32;
    typedef double          f64;
#endif

typedef struct {
    s32 major;		///< significant changes
    s32 minor;		///< incremental changes
    s32 revision;	///< bug fixes
}StarVersion;

// p = pointer buffer
// s = size
// f = file pointer

#ifdef STAR_IOS_VERSION
    //#import <OpenGLES/ES1/gl.h>
    //#import <OpenGLES/ES1/glext.h>

    #define  SFILE  FILE
    #define READ_FILE(p,s,f) {fread(p,s,1,f);}
    #define SEEK_FILE(f,p,s) {fseek(f,p,s);}
    #define CLOSE_FILE(x) {fclose(x);}
#elif defined(STAR_ANDROID_VERSION)
    #include <jni.h>
    //#include <GLES/gl.h>
    //#include <GLES/glext.h>
    //#include <EGL/egl.h>
    #include <android/log.h>
// File Macro
    #ifdef USE_APK_ZIP
        #include <zip.h>
        #define SFILE zip_file
        #define READ_FILE(p,s,f) {zip_fread(f,p,s);}
        #define CLOSE_FILE(x) {zip_close(x);}
    #else
    // for native asset manager
        #include <sys/types.h>
        //#include <android/asset_manager.h>
        //#include <android/asset_manager_jni.h>
        #define SFILE AAsset
        #define READ_FILE(p,s,f) {AAsset_read(f,p,s);}
        #define SEEK_FILE(f,p,s) {AAsset_seek(f,p,s);}
        #define CLOSE_FILE(x) {AAsset_close(x);}
    #endif

#elif defined(__APPLE__)
    //#include <OpenGL/OpenGL.h>
    //#include <OpenGL/gl.h>
    //#include <OpenGL/glu.h>
    //#include <OpenGL/glext.h>
    #define  SFILE  FILE
    #define READ_FILE(p,s,f) {fread(p,s,1,f);}
    #define SEEK_FILE(f,p,s) {fseek(f,p,s);}
    #define CLOSE_FILE(x) {fclose(x);}

#elif defined(STAR_WINDOWS_VERSION)
   // #include "freeglut/freeglut.h"
   // #include <GL/glut.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
	#include <windows.h>
	#include <WinSock2.h>
	#include <WS2tcpip.h>
	#include <IPHlpApi.h>
	#include <stdio.h>
	#pragma comment(lib,"Ws2_32.lib");
	#pragma comment(lib,"Winmm.lib");
	//#pragma comment(lib,"opengl32.lib");
	//#include <GL/gl.h>
	//#include <GL/glut.h>
    #define  SFILE  FILE
    #define READ_FILE(p,s,f) {fread(p,s,1,f);}
    #define SEEK_FILE(f,p,s) {fseek(f,p,s);}
    #define CLOSE_FILE(x) {fclose(x);}
#endif

//STAR_IOS_VERSION



#ifdef STAR_DEBUG
    #ifdef STAR_ANDROID_VERSION

//        #ifdef DT_DEBUG
//            #undef DT_DEBUG
//        #endif

        #define  LOG_TAG    "libStar"
        #define SLOG(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
    #else
        #define SLOG(...) printf(__VA_ARGS__)
    #endif


//    #ifdef DT_DEBUG
//
//        #ifdef STAR_ANDROID_VERSION
//            #define DTLOG(...) ((void)0)
//        #elif STAR_IOS_VERSION
//            #define DTLOG(...) ((void)0)
//        #else
//            #define DTLOG(fmt, ...)    \
//            do { \
//                static char buf[4096] = "hahaha\n"; \
//                sprintf(buf, fmt, ##__VA_ARGS__); \
//                DebugLog(buf); \
//            } while (0)
//        #endif
//
//    #else
//        #define DTLOG(...) ((void)0)
//    #endif


#else
    #define SLOG(...) ((void)0)
    //#define DTLOG(...) ((void)0)
#endif


#endif
