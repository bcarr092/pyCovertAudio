/*! \file   osx_cahal_device_stream.h
    \brief  The platform-specific functions to query the hardware for its
            hardware-supported device streams as well as manage device streams
            are located in this file.
 
    \author Brent Carrara
 */
#ifndef __OSX_CAHAL_DEVICE_STREAM_H__
#define __OSX_CAHAL_DEVICE_STREAM_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal.h"

#include "osx_cahal_device.h"
#include "osx_cahal_audio_format_description.h"

#include "darwin/darwin_cahal_audio_format_description.h"

/*! \fn     OSStatus osx_get_device_streams  (
              cahal_device* io_device
            )
    \brief  Populate the list of streams supported by the audio device
            identified by the handle in io_device. The caller must free the list
            of streams. The list is null-terminated. Streams provide a direction
            and preferred format as well as supported formats to the caller as
            well as a handler to the stream.

    \param  io_device The streams supported by the audio device pointed to by
                      the handle in io_device will be queried and their
                      attributes including handle, direction and supported
                      formats will be stored in the list of streams in
                      io_device. This is a null terminated list and must be
                      freed by the caller.
    \return Either noErr(0) or an error code. The error code can be passed to
            printed for debugging purposes. Not the error code returned is a
            platform specific error code.
 */
OSStatus
osx_get_device_streams  (
                         cahal_device* io_device
                         );

/*! \fn     void osx_set_cahal_device_stream_struct(
              AudioStreamID          in_device_stream_id,
              cahal_device_stream*   io_device_stream
            )
    \brief  Populates io_device_stream with all the stream's properties by
            querying the properties of the passed in stream handler as well as
            the preferred and supportd audio formats that the stream referenced
            by in_device_stream_id points to.
 
    \param  in_device_stream_id The id whose properties are to be queried and
                                populated in in_device_stream.
    \param  io_device_stream  The struct to be populated.
 */
void
osx_set_cahal_device_stream_struct(
                                   AudioStreamID          in_device_stream_id,
                                   cahal_device_stream*   io_device_stream
                                   );

/*! \fn     OSStatus osx_get_device_stream_supported_formats (
              cahal_device_stream* io_device_stream
            )
    \brief  Queries the hardware stream referenced in io_device_stream for its
            hardware preferred audio format, e.g. lpcm, as well as catalogues
            all the hardware supported parameters, e.g. format, sample rate,
            number of channels, etc as well. A null-terminated list of audio
            formats will be created by this function, which must be freed by the
            caller. Note that the list of supported formats created by calling
            this function must be freed by the caller upon success.
 
    \param  io_device_stream  The device stream whose preferred and supported
                              formats are to be queried and populated. The
                              null-terminated list of supported streams must be
                              freed by the caller. The handle field of
                              io_device_stream must be properly populated before
                              calling this function. The list of supported
                              formats must be freed by the caller.
    \return noErr(0) if both the supported and preferred audio formats cane be
            queired and error code otherwise. Note this is a platform specific
            error code.
 */
OSStatus
osx_get_device_stream_supported_formats (
                                         cahal_device_stream* io_device_stream
                                         );

#endif  /*  __OSX_CAHAL_DEVICE_STREAM_H__ */
