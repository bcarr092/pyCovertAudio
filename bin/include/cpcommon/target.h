/*! \file   target.h
    \brief  File that determines the platform the cpcommon library is being
            built for. This file deteremines the specific platform through
            preprocessor defines.
 
    \author Brent Carrara
 */
#ifndef _TARGET_H_
#define _TARGET_H_

#ifdef _WIN32
  #ifdef _WIN64
    /*! \def    __WIN64__
        \brief  Defined when building for 64-bit Windows
     */
    #define __WIN64__
  #else
    /*! \def    __WIN32__
        \brief  Defined when building for 32-bit Windows
     */
    #define __WIN32__
  #endif
#elif __APPLE__
    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR
      /*! \def    __IOS_SIM__
          \brief  Defined when building for the iPhone simulator
       */
      #define __IOS_SIM__
    #elif TARGET_OS_IPHONE
      /*! \def    __IOS__
          \brief  Defined when building for the iPhone
       */
      #define __IOS__
    #elif TARGET_OS_MAC
      /*! \def    __OSX__
          \brief  Defined when building for Mac OS X
       */
      #define __OSX__
    #else
      #error "Unknown Apple platform"
    #endif
#elif __ANDROID__
  /* Do nothing, we support this platform */
#elif __linux
    #error "Linux is unsupported"
#elif __unix /* all unixes not caught above */
    #error "General Unix is unsupported"
#elif __posix
    #error "Platform unsupported"
#endif

#endif /* _TARGET_H_ */
