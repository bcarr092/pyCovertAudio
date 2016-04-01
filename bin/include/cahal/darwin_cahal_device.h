/*! \file   darwin_cahal_device.h
    \brief  Common code for iOS and OSX to handle recording/playback.
 
    \author Brent Carrara
 */
#ifndef __DARWIN_CAHAL_DEVICE_H__
#define __DARWIN_CAHAL_DEVICE_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal.h"
#include "cahal_device.h"
#include "cahal_device_stream.h"
#include "cahal_audio_format_flags.h"

#include "darwin_cahal_audio_format_flags.h"
#include "darwin_cahal_audio_format_description.h"

/*! \var    darwin_context
    \brief  This is the platform-specific struct that stores the references
            required to release the audio device back to the OS.
 */
typedef struct darwin_context_t
{
  /*! \var    audio_queue
      \brief  The audio queue used to record/playback audio samples.
   */
  AudioQueueRef         audio_queue;
  
  /*! \var    number_of_buffers
      \brief  The number of buffers in audio_buffers. This is required when
              freeing the audio buffers.
   */
  UINT32                number_of_buffers;
  
  /*! \var    audio_buffers
      \brief  The audio buffers used to record/playback samples.
   */
  AudioQueueBufferRef*  audio_buffers;
  
} darwin_context;

/*! \fn     void darwin_recorder_callback  (
              void*                                in_user_data,
              AudioQueueRef                        in_queue,
              AudioQueueBufferRef                  in_buffer,
              const AudioTimeStamp*                in_start_time,
              UINT32                               in_number_of_packets,
              const AudioStreamPacketDescription*  in_packet_description
            )
    \brief  This the callback that is passed to the Core Audio library, which is
            called when a buffer of recorded samples is ready for processing.
    
    \param  in_user_data  This is a pointer to a cahal_recorder_info struct. It
                          contains the device that the recording was made from
                          as well as the callback function that should be called
                          with the recorded buffer.
    \param  in_queue  A reference to the queue containing the buffer of recorded
                      samples.
    \param  in_buffer The buffer of recorded samples. Note that the buffer
                      contains samples encoded in the format provided when
                      cahal_start_recording was called.
    \param  in_start_time Timestamp the recording was made at.
    \param  in_number_of_packets  The number of packets in the recording. Note
                                  that this is not the same as the number of
                                  bytes in the packet. At this point only
                                  constant bit rate codes are supported so this
                                  value is always used to calculate the number
                                  of bytes in in_buffer.
    \param  in_packet_description Format information for the packet. Only used
                                  with VBR codecs. Always null in CBR cases.
 */
static
void
darwin_recorder_callback  (
                      void*                                in_user_data,
                      AudioQueueRef                        in_queue,
                      AudioQueueBufferRef                  in_buffer,
                      const AudioTimeStamp*                in_start_time,
                      UINT32                               in_number_of_packets,
                      const AudioStreamPacketDescription*  in_packet_description
                        );

/*! \fn     void darwin_playback_callback (
              void*                in_user_data,
              AudioQueueRef        in_queue,
              AudioQueueBufferRef  in_buffer
            )
    \brief  This the callback that is passed to the Core Audio library, which is
            called when a buffer of samples needs to be populated for playback.

    \param  in_user_data  This is a pointer to a cahal_playback_info struct. It
                          contains the device that the playback is to made on
                          as well as the callback function that should be called
                          to fill in_buffer.
    \param  in_queue  A reference to the queue containing the buffer of playback
                      samples.
    \param  in_buffer The empty buffer to be filled with samples. Note that the
                      buffer must be filled with samples encoded in the format
                      provided when cahal_start_playback was called.
 */
static
void
darwin_playback_callback (
                       void*                in_user_data,
                       AudioQueueRef        in_queue,
                       AudioQueueBufferRef  in_buffer
                       );

/*! \fn     OSStatus darwin_configure_asbd  (
              cahal_audio_format_id          in_format_id,
              UINT32                         in_number_of_channels,
              FLOAT64                        in_sample_rate,
              UINT32                         in_bit_depth,
              cahal_audio_format_flag        in_format_flags,
              cahal_device_stream_direction  in_direction,
              AudioStreamBasicDescription    *out_asbd
            )
    \brief  Helper function to create the AudioStreamBasicDescrition. This is
            the platform specific model that is populated with the format
            information and parameters. The ASBD is used in both input and
            output operations.
 
    \param  in_format_id  The CAHAL format that the samples will be encoded in
                          for playback/recording, e.g. lpcm.
    \param  in_number_of_channels The number of channels in the stream.
    \param  in_sample_rate  The number of samples to capture per second.
    \param  in_bit_depth  The quantization level, i.e. the number of bits per
                          sample.
    \param  in_format_flags Flags to further describe the encoding method. These
                            flags are CAHAL flags.
    \param  in_direction  The direction the ASBD is being built for, i.e. input
                          or output.
    \param  out_asbd  The newly populated ASBD if no error occurs.
 */
OSStatus
darwin_configure_asbd  (
                     cahal_audio_format_id          in_format_id,
                     UINT32                         in_number_of_channels,
                     FLOAT64                        in_sample_rate,
                     UINT32                         in_bit_depth,
                     cahal_audio_format_flag        in_format_flags,
                     cahal_device_stream_direction  in_direction,
                     AudioStreamBasicDescription    *out_asbd
                     );

/*! \fn     OSStatus darwin_compute_bytes_per_buffer  (
              AudioStreamBasicDescription* in_asbd,
              FLOAT32                      in_number_of_seconds,
              UINT32*                      out_bytes_per_buffer
            )
    \brief  Helper function to calculate number of bytes to put in each buffer
            based on the configured ASBD and in_number_of_seconds.
 
    \param  in_asbd The ASBD that contains the relevant encoding information,
                    e.g. number of channels, bit depth, sample rate.
    \param  in_number_of_seconds  The number of seconds that each buffer will
                                  represent.
    \param  out_bytes_per_buffer  The set number of bytes to put in each buffer.
    \return noErr(0) if no error occurs or an appropriate error code.
 */
OSStatus
darwin_compute_bytes_per_buffer  (
                             AudioStreamBasicDescription* in_asbd,
                             FLOAT32                      in_number_of_seconds,
                             UINT32*                      out_bytes_per_buffer
                               );

/*! \fn     OSStatus darwin_configure_input_audio_queue (
              cahal_device*                 in_device,
              cahal_recorder_info*          in_callback_info,
              AudioStreamBasicDescription*  io_asbd,
              AudioQueueRef*                out_audio_queue
            )
    \brief  Configures the platforms input (record) audio queue.
 
    \param  in_device The recording device.
    \param  in_callback_info  The callback that is called by the system when
                              a buffer of samples is ready for processing.
    \param  io_asbd The ASDB containing all the format and endocing information.
                    This value is somteimes updated by the queue when format
                    parameters are changed.
    \param  out_audio_queue The newly created platform-specific audio queue.
    \return noErr(0) if no error occurs or an appropriate error code.
 */
OSStatus
darwin_configure_input_audio_queue (
                                 cahal_device*                 in_device,
                                 cahal_recorder_info*          in_callback_info,
                                 AudioStreamBasicDescription*  io_asbd,
                                 AudioQueueRef*                out_audio_queue
                                 );

/*! \fn     OSStatus darwin_configure_output_audio_queue (
              cahal_device*                 in_device,
              cahal_playback_info*          in_callback_info,
              FLOAT32                       in_volume,
              AudioStreamBasicDescription*  in_asbd,
              AudioQueueRef*                out_audio_queue
            )
    \brief  Configures the platforms ouput (playback) audio queue.

    \param  in_device The playback device.
    \param  in_callback_info  The callback that is called by the system when
                              a buffer of samples needs to be filled.
    \param  in_volume Volume gain (value between 0 and 1). This attenuates the
                      output signal but is still subject to the max volume of
                      the device.
    \param  in_asbd The ASDB containing all the format and endocing information.
    \param  out_audio_queue The newly created platform-specific audio queue.
    \return noErr(0) if no error occurs or an appropriate error code.
    */
OSStatus
darwin_configure_output_audio_queue (
                                cahal_device*                 in_device,
                                cahal_playback_info*          in_callback_info,
                                FLOAT32                       in_volume,
                                AudioStreamBasicDescription*  in_asbd,
                                AudioQueueRef*                out_audio_queue
                                  );

/*! \fn     OSStatus darwin_configure_input_audio_queue_buffer  (
              AudioStreamBasicDescription* in_asbd,
              darwin_context*              out_context,
              AudioQueueRef                io_audio_queue
            )
    \brief  Allocates the necessary audio queue buffers and populates the queue
            with them.
 
    \param  in_asbd The ASBD containing the format and encoding information.
    \param  out_context Structure that stores references to the queue and buffers
                        so that they can be released. This function sets the
                        buffers in out_context.
    \param  io_audio_queue  The audio queue that is populated with buffers.
 */
OSStatus
darwin_configure_input_audio_queue_buffer  (
                                    AudioStreamBasicDescription* in_asbd,
                                    darwin_context*              out_context,
                                    AudioQueueRef                io_audio_queue
                                            );

/*! \fn     OSStatus darwin_configure_output_audio_queue_buffer  (
             AudioStreamBasicDescription*  in_asbd,
             cahal_playback_info*          in_playback,
             darwin_context*               out_context,
             AudioQueueRef                 io_audio_queue
            )
    \brief  Allocates the necessary audio queue buffers and populates the queue
            with them. This function will call the playback callback to
            initially populate the buffer queue with data.

    \param  in_asbd The ASBD containing the format and encoding information.
    \param  in_playback The user data struct containing device specific info
                        required by the playback callback.
    \param  out_context Structure that stores references to the queue and buffers
                        so that they can be released. This function sets the
                        buffers in out_context.
    \param  io_audio_queue  The audio queue that is populated with buffers.
    */
OSStatus
darwin_configure_output_audio_queue_buffer  (
                                   AudioStreamBasicDescription*  in_asbd,
                                   cahal_playback_info*          in_playback,
                                   darwin_context*               out_context,
                                   AudioQueueRef                 io_audio_queue
                                             );

/*! \fn     OSStatus darwin_free_context (
              darwin_context* io_context
            )
    \brief  Frees all the structs in io_context. Releases the audio queue and
            buffers back to the OS using the AudioQueue* API.
 
    \param  io_context  The audio queue and audio queue buffers to free.
    \return noErr(0) if all structs are freed, an appropriate error code
            otherwise.
 */
OSStatus
darwin_free_context (
                     darwin_context* io_context
                     );

#endif  /*  __DARWIN_CAHAL_DEVICE_H__ */
