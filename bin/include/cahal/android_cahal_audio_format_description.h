/*! \file   android_cahal_device_format_description.h
 *  \brief  Device format description conversion helper functions to convert
 *          between OpenSLES and CAHAL.

    \author Brent Carrara
 */
#ifndef __ANDROID_CAHAL_AUDIO_FORMAT_DESCRIPTION_H__
#define __ANDROID_CAHAL_AUDIO_FORMAT_DESCRIPTION_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include "cahal_audio_format_description.h"

/*! \fn     cahal_audio_format_id android_convert_android_audio_format_id_to_cahal_audio_format_id (
              UINT32 android_format_id
            )
    \brief  This function bridges the android audio format constants to the cahal
            format constants. It takes as input an OpenSL audio format id and
            returns a cahal format id.

    \param  android_format_id  The OpenSL audio format id to convert to a cahal
                               format id. This is a platform specific value.
    \return The equivalent cahal audio format id for the given OpenSL audio format
            id. This is a platform agnostic value.
 */
cahal_audio_format_id
android_convert_android_audio_format_id_to_cahal_audio_format_id (
                                                     UINT32 android_format_id
                                                                 );

/*! \fn     UINT32 android_convert_cahal_audio_format_id_to_android_audio_format_id (
              cahal_audio_format_id in_format_id
            )
    \brief  This function maps cahal audio format constants back to the platform
            specific constants supported by Android (OpenSLES).

    \param  in_format_id  The CAHAL audio format constant to be converted to
                          a OpenSL audio format constant. This is a platform
                          agnostic value.
    \return The core audio format constant representing in_format_id. This is a
            platform specific value.
 */
UINT32
android_convert_cahal_audio_format_id_to_android_audio_format_id (
                                             cahal_audio_format_id in_format_id
                                                                 );

#endif /* __ANDROID_CAHAL_AUDIO_FORMAT_DESCRIPTION_H__ */
