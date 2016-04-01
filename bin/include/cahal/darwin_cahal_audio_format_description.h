/*! \file   darwin_cahal_audio_format_description.h
    \brief  This is the platform-specific file responsible for querying the
            hardware for its supported audio formats as well as creating the
            list of supported formats for each device stream.
 
    \author Brent Carrara
 */
#ifndef __darwin_CAHAL_AUDIO_FORMAT_H__
#define __darwin_CAHAL_AUDIO_FORMAT_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal_audio_format_description.h"

/*! \fn     cahal_audio_format_id darwin_convert_core_audio_format_id_to_cahal_audio_format_id (
              UINT32 core_audio_format_id
            )
    \brief  This function bridges the core audio format constants to the cahal
            format constants. It takes as input a core audio format id and
            returns a cahal format id. The platform specific format valules are
            not bitmasks and must be checked for equality since only one value
            can be set at a time.
 
    \param  core_audio_format_id  The core audio format id to convert to a cahal
                                  format id. This is a platform specific value.
    \return The equivalent cahal audio format id for the given core audio format
            id. This is a platform agnostic value.
 */
cahal_audio_format_id
darwin_convert_core_audio_format_id_to_cahal_audio_format_id (
                                                     UINT32 core_audio_format_id
                                                           );

/*! \fn     UINT32 darwin_convert_cahal_audio_format_id_to_core_audio_format_id (
              cahal_audio_format_id in_format_id
            )
    \brief  This function maps cahal audio format constants back to the platform
            specific constants supported by Darwin. 
 
    \param  in_format_id  The CAHAL audio format constant to be converted to
                          a core audio format constant. This is a platform
                          agnostic value.
    \return The core audio format constant representing in_format_id. This is a
            platform specific value.
 */
UINT32
darwin_convert_cahal_audio_format_id_to_core_audio_format_id (
                                             cahal_audio_format_id in_format_id
                                                           );
#endif /* __darwin_CAHAL_AUDIO_FORMAT_H__ */
