/*! \file   osx_cahal_device.h
    \brief  This interface takes care of creating the platform-agnostic device
            structs as well as the implementation for the audio hardware
            controls, i.e. playback and record.
 
    \author Brent Carrara
 */
#ifndef __OSX_CAHAL_DEVICE_H__
#define __OSX_CAHAL_DEVICE_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal.h"
#include "cahal_device.h"
#include "cahal_device_stream.h"
#include "cahal_audio_format_flags.h"

#include "osx_cahal_device_stream.h"

/*! \fn     void osx_set_cahal_device_struct(
              AudioObjectID in_device_id,
              cahal_device* io_device
            )
    \brief  Populate the given io_device struct with the parameters of the
            audio hardware object defined by in_device_id. Note that this 
            function will catalogue all the supported streams of the device
            as well as all of the supported formats.
 
    \param  in_device_id  The device handle whose properties are to be populated
                          in io_device.
    \param  io_device     The cahal_device struct to populate with the
                          properties of in_device_id.
 */
void
osx_set_cahal_device_struct(
                            AudioObjectID in_device_id,
                            cahal_device* io_device
                            );

/*! \fn     OSStatus osx_get_device_uint32_property  (
              AudioObjectID                in_device_id,
              AudioObjectPropertySelector  in_property,
              UINT32*                      out_device_property
            )
    \brief  Extract the UINT32 value of in_property from the audio hardware
            in_device_id. The extracted value is stored in out_device_property.
            This is a helper function to wrap the OSX interface for audio 
            hardware.
 
    \param  in_device_id  The device handle whose property is to be extracted.
    \param  in_property   The property to query on in_device_id
    \param  out_device_property The location to store the extracted property
                                value.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_uint32_property  (
                               AudioObjectID                in_device_id,
                               AudioObjectPropertySelector  in_property,
                               UINT32*                      out_device_property
                                 );

/*! \fn     OSStatus osx_get_device_float64_property  (
              AudioObjectID                in_device_id,
              AudioObjectPropertySelector  in_property,
              FLOAT64*                     out_device_property
            )
    \brief  Extract the FLOAT64 value of in_property from the audio hardware
            in_device_id. The extracted value is stored in out_device_property.
            This is a helper function to wrap the OSX interface for audio
            hardware.

    \param  in_device_id  The device handle whose property is to be extracted.
    \param  in_property   The property to query on in_device_id
    \param  out_device_property The location to store the extracted property
                                value.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_float64_property (
                               AudioObjectID                in_device_id,
                               AudioObjectPropertySelector  in_property,
                               FLOAT64*                     out_device_property
                                 );

/*! \fn     OSStatus osx_get_device_string_property  (
              AudioObjectID                in_device_id,
              AudioObjectPropertySelector  in_property,
              CHAR**                       out_device_property
            )
    \brief  Extract the CHAR value of in_property from the audio hardware
            in_device_id. The extracted value is stored in out_device_property.
            This is a helper function to wrap the OSX interface for audio
            hardware.

    \param  in_device_id  The device handle whose property is to be extracted.
    \param  in_property   The property to query on in_device_id
    \param  out_device_property The location to store the extracted property
                                value.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_string_property  (
                                 AudioObjectID               in_device_id,
                                 AudioObjectPropertySelector in_property,
                                 CHAR**                      out_device_property
                                 );

/*! \fn     OSStatus osx_get_device_property_value  (
              AudioObjectID                in_device_id,
              AudioObjectPropertySelector  in_property,
              UINT32                       in_device_property_value_size,
              void*                        out_device_property_value
            )
    \brief  Generic function to extract the value for in_property from the audio
            hardware in_device_id. The extracted value is stored in
            out_device_property_value.
            This is a helper function to wrap the OSX interface for audio
            hardware.

    \param  in_device_id  The device handle whose property is to be extracted.
    \param  in_property   The property to query on in_device_id
    \param  in_device_property_value_size The number of bytes that will be
                                          stored in out_device_property_value.
    \param  out_device_property_value The location to store the extracted
                                      property value.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_property_value (
                     AudioObjectID                in_device_id,
                     AudioObjectPropertySelector  in_property,
                     UINT32                       in_device_property_value_size,
                     void*                        out_device_property_value
                               );

/*! \fn     OSStatus osx_get_device_property_size_and_value  (
              AudioObjectID                in_device_id,
              AudioObjectPropertySelector  in_property,
              UINT32*                      out_device_property_value_size,
              void**                       out_device_property_value
            )
    \brief  Generic function to extract both the size and value for in_property
            from the audio hardware in_device_id. The extracted size is stored
            in out_device_property_value_size and value is stored in
            out_device_property_value. This is a helper function to wrap the OSX
            interface for audio hardware.

    \param  in_device_id  The device handle whose property is to be extracted.
    \param  in_property   The property to query on in_device_id
    \param  out_device_property_value_size  The location to store the number of
                                            bytes required to store the property
                                            value.
    \param  out_device_property_value The location to store the extracted
                                      property value. The size of
                                      out_device_property_value is
                                      out_device_property_value_size. The caller
                                      must free this parameter.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_property_size_and_value (
                   AudioObjectID                in_device_id,
                   AudioObjectPropertySelector  in_property,
                   UINT32*                      out_device_property_value_size,
                   void**                       out_device_property_value
                                        );

/*! \fn     OSStatus osx_get_device_supported_sample_rates (
              cahal_device* io_device
            )
    \brief  Populate the list of supported sample rates in io_device. The
            caller must free the list of supported sample rates. The list is
            null-terminated. This is a helper function to wrap the OSX
            interface for audio hardware.
 
    \param  io_device The sample rates supported by the audio device pointed to
                      by the handle in io_device will be logged in the list of
                      sample rates in io_device. This is a null terminated list
                      and must be freed by the caller.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_device_supported_sample_rates (
                                       cahal_device* io_device
                                       );

/*! \fn     OSStatus osx_get_number_of_channels  (
              cahal_device* io_device
            )
    \brief  Populate the number of supported channels in the cahal_device. We
            query a number of different properties to find the correct number of
            channels supported by the device.
 
    \param  io_device The number of channels supported by the device will be
                      logged in the numeber_of_channels field of io_device.
    \return Either noErr or an error code. The error code can be passed to
            darwin_helper for printing.
 */
OSStatus
osx_get_number_of_channels  (
                             cahal_device* io_device
                             );

#endif /* __OSX_CAHAL_DEVICE_H__ */
