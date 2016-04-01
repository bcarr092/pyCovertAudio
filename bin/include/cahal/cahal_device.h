/*! \file     cahal_device.h
    \brief    The platform agnostic CAHAL device struct is defined in this file.
              All helper functions that are implemented in a platform-agnostic
              way are also defined here. A device struct models all properties
              of an audio device including streams (i.e. input/output) as well
              as format information including bit depth, and sample rate.
 
    \author Brent Carrara
 */
#ifndef __CAHAL_DEVICE_H__
#define __CAHAL_DEVICE_H__

#include <cpcommon.h>

#include "cahal_audio_format_flags.h"
#include "cahal_audio_format_description.h"
#include "cahal_device_stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \def    CAHAL_QUEUE_BUFFER_DURATION
    \brief  The amount of time (in seconds) to buffer samples for in each queued
            buffer.
 */
#define CAHAL_QUEUE_BUFFER_DURATION           1

/*! \def    CAHAL_QUEUE_NUMBER_OF_QUEUES
    \brief  The number of queued objects to store in the buffer queue (input and
            output) per device.
 */
#define CAHAL_QUEUE_NUMBER_OF_QUEUES          5

/*! \var    cahal_device_handle
    \brief  Type definition for device handles
 */
typedef UINT32 cahal_device_handle;

/*! \var    cahal_device
    \brief  Struct definition for devices
 */
typedef struct cahal_device_t
{
  /*! \var    device_name
      \brief  Printable name for the device
   */
  CHAR*               	device_name;
  
  /*! \var    model
      \brief  Printable model name of the device
   */
  CHAR*                 model;
  
  /*! \var    manufacturer
      \brief  Printable manufacturer name of the device
   */
  CHAR*                 manufacturer;
  
  /*! \var    serial_number
      \brief  Printable serial number of the device
   */
  CHAR*                 serial_number;
  
  /*! \var    version
      \brief  Printable version string for the device
   */
  CHAR*                 version;
  
  /*! \var    device_uid
      \brief  Printable universal ID for the device
   */
  CHAR*                 device_uid;
  
  /*! \var    model_uid
      \brief  Printable universal ID for the model
   */
  CHAR*                 model_uid;
  
  /*! \var    preferred_sample_rate
      \brief  The nominal sample rate for the device. This is the preferred
              the sample rate for the device. This rate is supported in hardware
   */
  FLOAT64               preferred_sample_rate;
  
  /*! \var    supported_sample_rates
      \brief  Array of supported sample rates in the form of
              cahal_sample_rate_ranges. This is a null-terminated list.
              This is a complete list of the sample rate ranges supported by the
              device. All supported sample rate ranges are supported by the
              audio device in hardware.

      \note   This needs to be removed. It is not used.
   */
  cahal_sample_rate_range** supported_sample_rates;
  
  /*! \var    device_streams
      \brief  Array of streams supported by the device. This is a null-
              terminated list. A stream represents a single input or output
              capability for the device. There is one stream per direction and a
              device capable of both input and output will have two streams.
              Presumably however a hardware device could have multiple inputs
              and/or outputs.
   */
  cahal_device_stream**     device_streams;
  
  /*! \var    handle
      \brief  OS-specific handle for the device. This should be treated as an
              opaque obect and never modified. The platform specific
              implementations use this value.
   */
  cahal_device_handle   handle;
  
  
  /*! \var    preferred_number_of_channels
      \brief  The preferred number of channels as defined by the audio hardware.
              This parameter is supported natively in hardware.
   */
  UINT32                preferred_number_of_channels;
  
  /*! \var    is_alive
      \brief  1 iff the device is ready to receive input/output. 0 otherwise.
   */
  UINT32                is_alive;
  
  /*! \var    is_running
      \brief  1 iff another process is actively using the device. 0 otherwise.
              Just because this value is set to 1 does not mean that another
              process cannot open the device for input/output.
   */
  UINT32                is_running;
  
} cahal_device;

/*! \var    g_device_list
    \brief  The global device list that is set the first time that
            cahal_get_device_list is called.
 */
extern cahal_device** g_device_list;

/*! \def    cahal_recorder_callback
    \brief  The function prototype for the callback used when buffers of
            recorded samples are available from an input device. The callback
            provides information related to the recording device as well as
            the filled in data buffer and its length (in bytes). The callback
            also passes in_client_data back to the caller who set up the
            callback unmodified.
 
    \note   The data in in_data_buffer is formatted according to the format
            that was passed into cahal_start_recording. The callback that
            receives the populated buffer is responsible for decoding.
            Similarly, the bit depth and number of channels must also be taken
            into account.
 
    \note   At this time only constant bit-rate codes are supported. When VBR
            codes are supported additional information will need to be passed
            back to the caller.
 
    \param  in_recording_device The device that the samples were recorded from.
    \param  in_data_buffer  The populated buffer filled with recorded samples.
                            The samples returned are in the format specified
                            when calling cahal_start_recording.
    \param  in_data_buffer_length The size of in_data_buffer in bytes.
    \param  in_client_data  User-supplied information that is passed back to the
                            caller unmodified.
 */
typedef CPC_BOOL (*cahal_recorder_callback) (
  cahal_device* in_recording_device,
  UCHAR*        in_data_buffer,
  UINT32        in_data_buffer_length,
  void*         in_client_data
);


/*! \def    cahal_playback_callback
    \brief  The function prototype for the callback used when buffers of
            playback samples are required by an output device. The callback
            provides information related to the playback device and passes an
            empty buffer to the callback to be populated. The callback that
            is also responsible for setting io_data_buffer_length to the number
            of bytes written to out_data_buffer. The original value of
            io_data_buffer is the maximum size (i.e. capacity) of
            out_data_buffer. The callback also passes in_client_data back to the
            caller who set up the callback unmodified.

    \note   The data in in_data_buffer must be formatted according to the format
            that was passed into cahal_start_playback. The callback that
            receives the empty buffer is responsible for encoding the data.
            The data encoded must be in the correct format, bit depth and sample
            rate.

    \note   At this time only constant bit-rate codes are supported. When VBR
            codes are supported additional information will need to be passed
            to the CAHAL library for playback.

    \param  in_playback_device The device that the samples will be played from.
    \param  out_data_buffer An empty buffer that needs to be populated by the
                            callback. The data must be properly encoded
                            according to the parameters passed into
                            cahal_start_playback.
    \param  io_data_buffer_length Originall the capacity (in bytes) of
                                  out_data_buffer. The callback is responsible
                                  for setting this value to the number of bytes
                                  written to out_data_buffer.
    \param  in_client_data  User-supplied information that is passed back to the
                            caller unmodified.
 */
typedef CPC_BOOL (*cahal_playback_callback) (
  cahal_device* in_playback_device,
  UCHAR*        out_data_buffer,
  UINT32*       io_data_buffer_length,
  void*         in_client_data
);

/*! \def    cahal_recorder_info
    \brief  Structure that is passed to the platform-specific callback routine
            when audio buffers populated with samples is made available by the
            recording device. This structure stores the ultimate callback that
            needs to be called after the data is copied from the OS' buffers
            as well as the user supplied data that was provided when the
            recording began.
 */
typedef struct cahal_recorder_info_t
{
  
  /*! \var    recording_device
      \brief  The device that was used to record the samples.
   */
  cahal_device*           recording_device;
  
  /*! \fn     recording_callback
      \brief  The ultimate function to be called once the recorded samples have
              been copied from the OS' buffers.
   */
  cahal_recorder_callback recording_callback;
  
  /*! \var    user_data
      \brief  The data that was supplied by the caller when the recording was 
              initiated.
   */
  void*                   user_data;
  
  /*! \var    platform_data
      \brief  Platform-specific pointer used to store platform specific data
              that is required by the callback that is called when a buffer is 
              full as well as to release control of the recording device back to
              the OS.
   */
  void*                   platform_data;
  
} cahal_recorder_info;

/*! \var    cahal_playback_info
    \brief  Structure that is passed to the platform-specific callback routine
            when audio buffers need to be populated with samples. This structure
            stores the ultimate callback that is called when data is required by
            the OS' buffers.
 */
typedef struct cahal_playback_info_t
{
  /*! \var    playback_device
      \brief  The device that was used to playback the samples.
   */
  cahal_device*             playback_device;
  
  /*! \fn     playback_callback
      \brief  The ultimate function to be called once the playback samples are
              required by the OS' buffers.
   */
  cahal_playback_callback   playback_callback;
  
  /*! \var    user_data
      \brief  The data that was supplied by the caller when the playback was
              initiated.
   */
  void*                     user_data;
  
  /*! \var    platform_data
      \brief  Platform-specific pointer used to store platform specific data
              that is required by the callback that is called when a buffer
              needs to be filled. Contains the info needed to to release control
              of the playback device back to the OS.
   */
  void*                     platform_data;
  
} cahal_playback_info;

/*! \fn     void cahal_print_device  (
              cahal_device* in_device
            )
    \brief  Prints in_device using the logger at the LOG level.
 
    \param  in_device The device to print.
 */
void
cahal_print_device  (
                     cahal_device* in_device
                     );

/*! \fn     void cahal_print_device_list (
              cahal_device** in_device_list
            )
    \brief  Prints all the devices in in__device_list
 
    \param  in_device_list  A null-terminated list of cahal_device structs that
                            is logged at the CPC_LOG_LEVEL_INFO level.
 */
void
cahal_print_device_list (
                         cahal_device** in_device_list
                         );

/*! \fn     cahal_device** cahal_get_device_list( void )
    \brief  Instantiates a list of audio input/output devices in a platform-
            agnostic way.

    \return A list of devices that can be used to interact with the OS'
            audio hardware. The device list is platform agnostic and must be
            freed by the caller.
 */
cahal_device**
cahal_get_device_list( void );

/*! \fn     void cahal_free_device_list()
    \brief  Frees the list of cahal_devices. This function will free all
            devices in the list as well as all members of those devices.

    \note   This function will free the global device list that is generated
            the first time you call cahal_get_device_list(). You should however
            never need to call this function directly as the global device list
            is freed when cahal_terminate is called.
 */
void
cahal_free_device_list( void );

/*! \fn     CPC_BOOL cahal_test_device_direction_support  (
              cahal_device*                 in_device,
              cahal_device_stream_direction in_direction
            )
    \brief  Tests to see if in_device is capable of supporting input or output
            depending on the value of in_direction. Input being recording (e.g.
            a mic) and output being playback (e.g. a speaker). This function
            will look for a stream in the in_device struct that supports the
            passed in direction.
 
    \param  in_device The device to test to see if it supports in_direction.
    \param  in_direction  Either input if in_device is being tested for 
                          recording support or output if in_device is being
                          tested for playback support.
    \return True iff in_device supports audio communication in the direction of
            in_direction, false otherwise.
 */
CPC_BOOL
cahal_test_device_direction_support  (
                                      cahal_device*                 in_device,
                                      cahal_device_stream_direction in_direction
                                      );

/*!  \fn     CPC_BOOL cahal_stop_recording( void )
   \brief  Calling this function will stop recording and free all structures
           related to the recording back to the OS. If no recording is taking
           place this function simply returns.
 
   \return True iff a recording has been stopped. Note that no recording is
           stopped if the library hasn't been initialized, or no recording is
           taking place.
 */
CPC_BOOL
cahal_stop_recording( void );

/*!  \fn     CPC_BOOL cahal_stop_playback( void )
   \brief  Calling this function will stop playback and free all structures
           related to the playback to the OS. If no playback is taking
           place this function simply returns false.
 
   \return True iff playback has been stopped. Note that no playback is
           stopped if the library hasn't been initialized, or no playback is
           taking place.
 */
CPC_BOOL
cahal_stop_playback( void );

/*! \fn     CPC_BOOL cahal_start_recording (
              cahal_device*            in_device,
              cahal_audio_format_id    in_format_id,
              UINT32                   in_number_of_channels,
              FLOAT64                  in_sample_rate,
              UINT32                   in_bit_depth,
              cahal_recorder_callback  in_recorder,
              void*                    in_callback_user_data,
              cahal_audio_format_flag  in_format_flags
            )
    \brief  The entry point into the CAHAL library to start recording. The
            caller is responsible for supplying all the necessary parameters.
            The most common reason for failure in this funtion is incorrect
            parameters. The caller is also responsible for supplying an
            appropriate callback which will receive a buffer of audio data per
            call back as well as any user data the caller wants when their
            callback is called.
 
    \param  in_device The device to record from.
    \param  in_format_id  The audio format to record in.
    \param  in_number_of_channels The number of channels to use in the recording
                                  e.g. 1 = mono, 2 = stereo.
    \param  in_sample_rate  The sample rate to record at.
    \param  in_bit_depth  The number of bits per sample to use (this is the
                          quantization level).
    \param  in_recorder The caller-supplied callback to be called with populated
                        buffers of recorded data are available.
    \param  in_callback_user_data The caller-supplied data to be passed back to
                                  the caller (unmodified) when their callback is
                                  called.
    \param  in_format_flags The flags to be used in the recording. Note that
                            these flags are converted back to the native OS'
                            flags. The most common reason why recording fails
                            is because these flags are incorrect,
                            i.e. unsupported.
    \return True iff recording has been start on in_device, false otherwise.
 */
CPC_BOOL
cahal_start_recording (
                       cahal_device*            in_device,
                       cahal_audio_format_id    in_format_id,
                       UINT32                   in_number_of_channels,
                       FLOAT64                  in_sample_rate,
                       UINT32                   in_bit_depth,
                       cahal_recorder_callback  in_recorder,
                       void*                    in_callback_user_data,
                       cahal_audio_format_flag  in_format_flags
                       );

/*! \fn     CPC_BOOL cahal_start_playback (
              cahal_device*            in_device,
              cahal_audio_format_id    in_format_id,
              UINT32                   in_number_of_channels,
              FLOAT64                  in_sample_rate,
              UINT32                   in_bit_depth,
              FLOAT32                  in_volume,
              cahal_playback_callback  in_playback,
              void*                    in_callback_user_data,
              cahal_audio_format_flag  in_format_flags
            )
    \brief  The entry point into the CAHAL library to start playback. The
            caller is responsible for supplying all the necessary parameters.
            The most common reason for failure in this funtion is incorrect
            parameters. The caller is also responsible for supplying an
            appropriate callback which will receive an empty buffer that the
            callback is required to fill with audio samples as well as any user
            data the caller wants when their callback is called.

    \param  in_device The device to playback to.
    \param  in_format_id  The audio format that samples are encoded in.
    \param  in_number_of_channels The number of channels to use in the playback
                                  e.g. 1 = mono, 2 = stereo.
    \param  in_sample_rate  The sample rate to playback at.
    \param  in_bit_depth  The number of bits per sample to use (this is the
                          quantization level).
    \param  in_volume Volume gain (value between 0 and 1). This attenuates the
                      output signal but is still subject to the max volume of
                      the device.
    \param  in_playback The caller-supplied callback to be called with empty
                        buffers that need to be filled.
    \param  in_callback_user_data The caller-supplied data to be passed back to
                                  the caller (unmodified) when their callback is
                                  called.
    \param  in_format_flags The flags to be used in the playback. Note that
                            these flags are converted back to the native OS'
                            flags. The most common reason why recording fails
                            is because these flags are incorrect,
                            i.e. unsupported.
    \return True iff playback has been start on in_device, false otherwise.
 */
CPC_BOOL
cahal_start_playback  (
                       cahal_device*            in_device,
                       cahal_audio_format_id    in_format_id,
                       UINT32                   in_number_of_channels,
                       FLOAT64                  in_sample_rate,
                       UINT32                   in_bit_depth,
                       FLOAT32                  in_volume,
                       cahal_playback_callback  in_playback,
                       void*                    in_callback_user_data,
                       cahal_audio_format_flag  in_format_flags
                       );

#ifdef __cplusplus
}
#endif

#endif  /*  _CAHAL_DEVICE_H__ */
