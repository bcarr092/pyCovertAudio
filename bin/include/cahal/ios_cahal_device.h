/*! \file   ios_cahal_device.h
    \brief  iOS platform specific calls to generate the device struct.
 
    \author Brent Carrara
 */
#ifndef __IOS_CAHAL_DEVICE_H__
#define __IOS_CAHAL_DEVICE_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal.h"
#include "cahal_device.h"
#include "cahal_device_stream.h"
#include "cahal_audio_format_flags.h"

#include "ios_cahal_device_stream.h"

/*! \def    IOS_DEVICE_HANDLE_OUTPUT
    \brief  The default device handle used to keep track of the output device.
 */
#define IOS_DEVICE_HANDLE_OUTPUT  1

/*! \def    IOS_DEVICE_HANDLE_INPUT
    \brief  The default device handle used to keep track of the input device.
 */
#define IOS_DEVICE_HANDLE_INPUT   2

/*! \def    IOS_DEVICE_STREAM_DEFAULT_HANDLE
    \brief  The default device handle used to keep track of device streams.
 */
#define IOS_DEVICE_STREAM_DEFAULT_HANDLE  1

/*! \fn     cahal_device** ios_set_cahal_device_struct( void )
    \brief  Mallocs and populates a newly created cahal_device struct. Note that
            the returned structure is an array of pointer references. This list
            is null-terminated. The list contains either one entry (if input is
            not supported) or two entries (if input is supported).
 
    \return A newly created null-terminated array. Either with one entry (if
            input is not supported) or two entries (if input is supported). NULL
            if the device could not be created.
 */
cahal_device**
ios_set_cahal_device_struct( void );

/*! \fn     BOOL ios_query_input_support( void )
    \brief  Query the audio session to determine if audio input is supported.
            Note that even if the hardware supports input (recording) the mode
            and category of the session need to be appropriately set to enable
            access to the input hardware.
 
    \return True iff audio input is support, false otherwise.
 */
CPC_BOOL
ios_query_input_support( void );

/*! \fn     OSStatus ios_get_device_uint32_property  (
              AudioSessionPropertyID  in_property,
              UINT32*                 out_device_property
            )
    \brief  Extract the UINT32 value of in_property from the audio session
            The extracted value is stored in out_device_property. This is a
            helper function to wrap the iOS interface for audio session.

    \param  in_property   The property to query
    \param  out_device_property The location to store the extracted property
                                value.
    \return Either kAudioSessionNoError or an error code. The error code can be 
            passed to darwin_helper for printing.
 */
OSStatus
ios_get_device_uint32_property  (
                                 AudioSessionPropertyID  in_property,
                                 UINT32*                 out_device_property
                                 );

/*! \fn     OSStatus ios_get_device_float64_property  (
              AudioSessionPropertyID  in_property,
              FLOAT64*                 out_device_property
            )
    \brief  Extract the FLOAT64 value of in_property from the audio session
            The extracted value is stored in out_device_property. This is a
            helper function to wrap the iOS interface for audio session.

    \param  in_property   The property to query
    \param  out_device_property The location to store the extracted property
                                value.
    \return Either kAudioSessionNoError or an error code. The error code can be
            passed to darwin_helper for printing.
 */
OSStatus
ios_get_device_float64_property (
                                 AudioSessionPropertyID  in_property,
                                 FLOAT64*                out_device_property
                                 );

/*! \fn     OSStatus ios_get_device_property_value (
              AudioSessionPropertyID in_property,
              UINT32*                io_device_property_value_size,
              void*                  out_device_property_value
            )
    \brief  Generic function to extract the value for in_property from the audio
            session. The extracted value is stored in out_device_property_value.
            This is a helper function to wrap the iOS interface for audio
            session.

    \param  in_property   The property to query on in_device_id
    \param  io_device_property_value_size The number of bytes that will be
                                          stored in out_device_property_value.
    \param  out_device_property_value The location to store the extracted
                                      property value.
    \return Either kAudioSessionNoError or an error code. The error code can be
            passed to darwin_helper for printing.
 */
OSStatus
ios_get_device_property_value (
                           AudioSessionPropertyID in_property,
                           UINT32*                io_device_property_value_size,
                           void*                  out_device_property_value
                               );

/*! \fn     OSStatus ios_get_device_name (
              CFStringRef  in_route_direction,
              CHAR**       out_device_label
            )
    \brief  Extract the device's name from the list of hardware supported in the
            audio session. This function assumes that there is only one device
            for a given direction (in_route_direction).
 
    \param  in_route_direction  The direction to extract the device name for.
    \param  out_device_label  The location to store the device name. NULL if one
                              cannot be extracted.
    \return Either kAudioSessionNoError or an error code. The error code can be
            passed to darwin_helper for printing.
 */
OSStatus
ios_get_device_name (
                     CFStringRef  in_route_direction,
                     CHAR**       out_device_label
                     );

/*! \fn     BOOL ios_get_input_device_info (
              cahal_device* out_input_device
            )
    \brief  Configures the only built in input device supported by iOS. This
            sets the minimum required fields (e.g. handle, device name, sample
            rate, and number of channels).
 
    \note   CAHAL currently does not support USB microphones or speakers on the
            iOS platform.
 
    \param  out_input_device  The input device to configure.
    \return True iff the minimum fields are set, false otherwise. Note that if
            False is returned the device is unusable.
 */
CPC_BOOL
ios_get_input_device_info (
                           cahal_device* out_input_device
                           );

/*! \fn     BOOL ios_get_output_device_info (
              cahal_device* io_output_device
            )
    \brief  Configures the only built in output device supported by iOS. This
            sets the minimum required fields (e.g. handle, device name, sample
            rate, and number of channels).
 
    \note   CAHAL currently does not support USB microphones or speakers on the
            iOS platform.

    \param  io_output_device  The input device to configure.
    \return True iff the minimum fields are set, false otherwise. Note that if
            False is returned the device is unusable.
    */
CPC_BOOL
ios_get_output_device_info (
                            cahal_device* io_output_device
                            );

/*! \fn     BOOL ios_set_device_stream (
              cahal_device* io_device,
              cahal_device_stream_direction in_direction
            )
    \brief  Configures the default input/output stream depending on in_direction
            This function sets the minimum fields required for a stream (e.g.
            handle, format, and direction).
 
    \note   CAHAL currently does not support USB microphones or speakers on the
            iOS platform.
 
    \param  io_device The device whos stream is to be set. Only one stream is
                      set for each device.
    \param  in_direction  The direction of the stream to create.
    \return Truee iff the minimum fields are set, false otherwise. Note that if
            False is returned the device stream is unusable.
 */
CPC_BOOL
ios_set_device_stream (
                       cahal_device* io_device,
                       cahal_device_stream_direction in_direction
                       );

/*! \fn     void ios_initialize_recording( void )
    \brief  Initializes the AVAudioSession to the appropriate category and mode
            so that input and output are possible.
 */
void
ios_initialize_recording( void );

/*! \fn     void ios_terminate_recording()
    \brief  Terminates the AVAudioSession session by setting the device to
            inactive.
 */
void ios_terminate_recording( void );

#endif  /*  __IOS_CAHAL_DEVICE_H__  */
