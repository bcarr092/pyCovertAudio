/*! \file   darwin_cahal_audio_format_flags.h
    \brief  Helper functions to convert CAHAL audio format flags to core audio
            flags and vice-versa.
 
    \author Brent Carrara
 */
#ifndef __darwin_CAHAL_AUDIO_FORMAT_FLAGS_H__
#define __darwin_CAHAL_AUDIO_FORMAT_FLAGS_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal_audio_format_flags.h"

/*! \fn     cahal_audio_format_flag darwin_convert_core_audio_format_flags_to_cahal_audio_format_flags (
              UINT32 core_audio_format_flags
            )
    \brief  Converts core audio format flags to platform agnostic CAHAL format
            flags. Note that the platform specific core audio flags are not a
            bit mask and therefore the constants are not orthogonal to one
            another. Therefore each flag needs to be tested for individually.
 
    \param  core_audio_format_flags The core audio format flags to convert to
                                      a CAHAL format flags.
    \return The CAHAL representation of in_core_audio_format_flags.
 */
cahal_audio_format_flag
darwin_convert_core_audio_format_flags_to_cahal_audio_format_flags (
                                               UINT32 core_audio_format_flags
                                                               );

/*! \fn     UINT32 darwin_convert_cahal_audio_format_flags_to_core_audio_format_flags (
              cahal_audio_format_flag in_format_flags
            )
    \brief  Converts CAHAL format flags to platform specific flags. Note that
            the platform agnostic CAHAL flags do form a bitmask of supported
            flags and therefore are orthogonal to one another.
 
    \param  in_format_flags  The format flags to convert to core audio format
                            flags. These are platform-specific flags.
    \return The core audio format flag representation of in_format_flag.
 */
UINT32
darwin_convert_cahal_audio_format_flags_to_core_audio_format_flags (
                                         cahal_audio_format_flag in_format_flags
                                                               );

#endif  /*  __darwin_CAHAL_AUDIO_FORMAT_FLAGS_H__  */
