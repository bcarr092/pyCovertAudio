/*! \file   cahal_audio_format_flags.h
    \brief  List of platform agnostic audio format flags and helper functions to
            display their value.
 
    \author Brent Carrara
 */
#ifndef __CAHAL_AUDIO_FORMAT_FLAGS_H__
#define __CAHAL_AUDIO_FORMAT_FLAGS_H__

#include <cpcommon.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*! \enum     cahal_audio_format_flags
    \brief    List of the various flags that are supported by the CAHAL library.
              These are platform-agnostic constants and are mapped to platform-
              specific constants in the platform-specific implementation.
 */
enum cahal_audio_format_flags
{
  CAHAL_AUDIO_FORMAT_FLAGISFLOAT                = ( 1 << 0 ),
  CAHAL_AUDIO_FORMAT_FLAGISBIGENDIAN            = ( 1 << 1 ),
  CAHAL_AUDIO_FORMAT_FLAGISSIGNEDINTEGER        = ( 1 << 2 ),
  CAHAL_AUDIO_FORMAT_FLAGISPACKED               = ( 1 << 3 ),
  CAHAL_AUDIO_FORMAT_FLAGISALIGNEDHIGH          = ( 1 << 4 ),
  CAHAL_AUDIO_FORMAT_FLAGISNONINTERLEAVED       = ( 1 << 5 ),
  CAHAL_AUDIO_FORMAT_FLAGISNONMIXABLE           = ( 1 << 6 ),
  CAHAL_AUDIO_FORMAT_FLAGSAREALLCLEAR           = ( 1 << 30 ),
  CAHAL_AUDIO_FORMAT_FLAGUNKNOWN                = ( 1 << 31 ),
};

#define CAHAL_AUDIO_FORMAT_STRING_FLAGISNONINTERLEAVED	"FlagIsNonInterleaved"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGSAREALLCLEAR      "FlagsAreAllClear"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISFLOAT           "FlagIsFloat"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISBIGENDIAN       "FlagIsBigEndian"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISPACKED          "FlagIsPacked"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISNONMIXABLE      "FlagIsNonMixable"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISALIGNEDHIGH     "FlagIsAlignedHigh"
#define CAHAL_AUDIO_FORMAT_STRING_FLAGISSIGNEDINTEGER   "FlagIsSignedInteger"

/*! \def    cahal_audio_format_flag
    \brief  Type definition for the various supported audio format flags.
 */
typedef UINT32 cahal_audio_format_flag;

/*! \fn     CHAR* cahal_convert_audio_format_flag_to_cstring  (
              cahal_audio_format_flag in_audio_format_flag
            )
    \brief  Returns the string representation of the flag represented by
            in_audio_format_flag.
 
    \param  in_audio_format_flag  The flag whose string representation is 
                                  returned. This is a platform-agnostic CAHAL
                                  flag.
    \return The string representation of in_audio_format_flag. The caller does
            not need to free this value.
 */
CHAR*
cahal_convert_audio_format_flag_to_cstring  (
                                    cahal_audio_format_flag in_audio_format_flag
                                              );

/*! \fn     void cahal_print_audio_format_flag (
              CHAR*                    in_label,
              cahal_audio_format_flag  in_format_flag
            )
    \brief  Logs the string representation of in_format_flag alogn with 
            in_label.
 
    \param  in_label  The string to print before the string representation of
                      in_format_flag. This is a platform-agnostic CAHAL flag.
    \param  in_format_flag  This parameter is converted to a string and logged
                            using the logger.
 */
void
cahal_print_audio_format_flag (
                               CHAR*                    in_label,
                               cahal_audio_format_flag  in_format_flag
                               );

#ifdef __cplusplus
}
#endif

#endif  /*  __CAHAL_AUDIO_FORMAT_FLAGS_H__  */
