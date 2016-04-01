/*! \file   osx_cahal_audio_format_description.h
    \brief  This is the platform-specific file responsible for querying the
            hardware for its supported audio formats as well as creating the
            list of supported formats for each device stream.
 
    \author Brent Carrara
 */
#ifndef __OSX_CAHAL_AUDIO_FORMAT_H__
#define __OSX_CAHAL_AUDIO_FORMAT_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal_audio_format_description.h"

#include "osx_cahal_device_stream.h"

/*! \fn     OSStatus osx_set_cahal_audio_format_description_list  (
              cahal_device_stream* io_device_stream
            )
    \brief  Queries the audio formats supported in hardware by io_device_stream
            and generates a null-temrinated list of
            cahal_audio_format_description structs for all supported formats.
            This list must be freed by the caller.
 
    \brief  io_device_stream  The device stream whose list of hardware supported
                              formats is to be queried for and whose list of
                              supported formats will be created and populated.
                              It is the responsibility of the caller to free
                              the list of supported formats created.
    \return noErr(0) if the supported formats property could be queried,
            an error code otherwise.
 */
OSStatus
osx_set_cahal_audio_format_description_list  (
                                          cahal_device_stream* io_device_stream
                                              );

#endif /* __OSX_CAHAL_AUDIO_FORMAT_H__ */
