/*! \file   types.h
    \brief  This is the cross-platform and cross-architecture standard types
            file. This header is automatically included by cpcommon.h.
 
    \author Brent Carrara
*/

#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "target.h"

#if defined( __OSX__ ) || defined( __IOS__ )
  #include <math.h>

  #include <CoreFoundation/CoreFoundation.h>

  #include "darwin/function_map.h"

  #ifdef __OSX__
    typedef int8_t    INT8;
    typedef int16_t   INT16;
    typedef int32_t   INT32;
    typedef int64_t   INT64;
    typedef uint8_t   UINT8;
    typedef uint16_t  UINT16;
    typedef UInt32    UINT32;
    typedef uint64_t  UINT64;
  #else
    typedef SInt8    INT8;
    typedef SInt16   INT16;
    typedef SInt32   INT32;
    typedef SInt64   INT64;
    typedef UInt8   UINT8;
    typedef UInt16  UINT16;
    typedef UInt32  UINT32;
    typedef UInt64  UINT64;
  #endif

  typedef Float32   FLOAT32;
  typedef Float64   FLOAT64;

  typedef char          CHAR;
  typedef unsigned char UCHAR;

  typedef ssize_t       SSIZE;
  typedef size_t        USIZE;
#elif defined( __ANDROID__ )
  #include <math.h>
  #include <errno.h>
  #include <ctype.h>

  #include <arpa/inet.h>

  #include "android/function_map.h"

  typedef int8_t    INT8;
  typedef int16_t   INT16;
  typedef int32_t   INT32;
  typedef int64_t   INT64;
  typedef uint8_t   UINT8;
  typedef uint16_t  UINT16;
  typedef uint32_t  UINT32;
  typedef uint64_t  UINT64;

  typedef float   FLOAT32;
  typedef double  FLOAT64;

  typedef char          CHAR;
  typedef unsigned char UCHAR;

  typedef ssize_t       SSIZE;
  typedef UINT32        USIZE;
#elif defined( __WIN32__ )
  #ifndef _DEBUG
    //#define _DEBUG
  #endif

  #ifdef _DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
  #endif

  #define _USE_MATH_DEFINES
  #include <math.h>

  #include <Windows.h>
  #include <windef.h>
  #include <winnt.h>
  #include <BaseTsd.h>

  #include "windows/function_map.h"

  typedef int8_t    INT8;
  typedef int16_t   INT16;
  typedef int32_t   INT32;
  typedef int64_t   INT64;
  typedef uint8_t   UINT8;
  typedef uint16_t  UINT16;
  typedef uint32_t  UINT32;
  typedef uint64_t  UINT64;

  typedef float   FLOAT32;
  typedef double  FLOAT64;

  typedef char          CHAR;
  typedef unsigned char UCHAR;

  typedef SSIZE_T       SSIZE;
  typedef size_t        USIZE;
#endif

/*! \def    CPC_TRUE
    \brief  True constant used by the CPCommon library.
 */
#define CPC_TRUE  1

/*! \def    CPC_FALSE
    \brief  False constant used by the CPCommon library.
 */
#define CPC_FALSE 0

/*! \var    CPC_BOOL
    \brief  We use a char to store boolean values.
 */
typedef UINT8 CPC_BOOL;

/*! \def    MAX_INT8
    \brief  Max signed byte value.
 */
#define MAX_INT8  SCHAR_MAX

/*! \def    MIN_INT8
    \brief  Min signed byte value.
 */
#define MIN_INT8  SCHAR_MIN

/*! \def    MAX_INT16
    \brief  Max signed short value.
 */
#define MAX_INT16 SHRT_MAX

/*! \def    MIN_INT16
    \brief  Min signed short value.
 */
#define MIN_INT16 SHRT_MIN

/*! \def    MAX_INT32
    \brief  Max signed 32-bit int value.
 */
#define MAX_INT32 INT_MAX

/*! \def    MIN_INT32
    \brief  Min signed 32-bit int value.
 */
#define MIN_INT32 INT_MIN

/*! \def    MAX_INT64
    \brief  Max signed 64-bit long value.
 */
#define MAX_INT64 LONG_MAX

/*! \def    MIN_INT64
    \brief  Min signed 64-bit long value.
 */
#define MIN_INT64 LONG_MIN

/*! \def    MAX_USIZE
    \brief  Max unsigned size value.
 */
#define MAX_USIZE SIZE_MAX

/*! \def    MIN_USIZE
    \brief  Min unsigned size value.
 */
#define MIN_USIZE 0

/*! \def    MAX_UINT8
    \brief  Max unsigned byte value.
 */
#define MAX_UINT8   UCHAR8

/*! \def    MIN_UINT8
    \brief  Min unsigned byte value.
 */
#define MIN_UINT8   0

/*! \def    MAX_UINT16
    \brief  Max unsigned short value.
 */
#define MAX_UINT16  USHRT_MAX

/*! \def    MIN_UINT16
    \brief  Min unsigned short value.
 */
#define MIN_UINT16  0

/*! \def    MAX_UINT32
    \brief  Max unsigned 32-bit int value.
 */
#define MAX_UINT32  UINT_MAX

/*! \def    MIN_UINT32
    \brief  Max unsigned 32-bit int value.
 */
#define MIN_UINT32  0

/*! \def    MAX_UINT64
    \brief  Max unsigned 64-bit long value.
 */
#define MAX_UINT64  ULONG_MAX

/*! \def    MIN_UINT64
    \brief  Min unsigned 64-bit long value.
 */
#define MIN_UINT64  0

#endif /* _TYPES_H_ */

