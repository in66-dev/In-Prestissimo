#ifndef INCLUDE_INDEBUG_H
#define INCLUDE_INDEBUG_H
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#ifdef __cplusplus
extern "C"{
#endif
#ifdef __APPLE__
    #include "TargetConditionals.h"
#if TARGET_OS_IPHONE
#define BUILD_FOR_IOS
#endif
#endif
#define DEBUG_ON
#ifdef DEBUG_ON
#ifdef __ANDROID__
#include <android/log.h>
#define INPRINT(format, ...) __android_log_print(ANDROID_LOG_ERROR, "InPrestissimo", format,##__VA_ARGS__)
#define INPRINT_FL(format,...) __android_log_print(ANDROID_LOG_INFO, "InPrestissimo", format", FUNC: %s, LINE: %d \n",##__VA_ARGS__, __func__, __LINE__)
#else
#define INPRINT(format, ...) printf(format,##__VA_ARGS__)
#define INPRINT_FL(format,...) printf(format", FUNC: %s, LINE: %d, in %s \n", ##__VA_ARGS__,__func__, __LINE__, __FILE__)
#endif
#define FUNC_PRINT(x) INPRINT(#x"=%d in %s, %d, %s \n",x,  __func__, __LINE__, __FILE__);
#define FUNC_PRINT_ALL(x, type) INPRINT(#x" = "#type" %"#type" in %s, %d, %s \n",x,  __func__, __LINE__, __FILE__);
#define CHECK_POINTER(x) {if(NULL==x){FUNC_PRINT_ALL(x,p);break;}}
    
#define INIFNOTLOG(x, info) {int result = (x); if (!(result)) FUNC_PRINT_ALL(info, s);}
#if defined(__ANDROID__) || defined(BUILD_FOR_IOS)
#define INASSERT(x) {int result = (x); if (!(result)) FUNC_PRINT((result));}
#else
#define INASSERT(x) assert(x)
#endif
#else
#define FUNC_PRINT(x)
#define FUNC_PRINT_ALL(x, type)
#define CHECK_POINTER(x)
#endif
#ifdef __cplusplus
}
#endif
#endif
