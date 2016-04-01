/*! \file   android_cahal_device.h
 *  \brief  Android specific OpenSLES API calls used to setup the list of
 *          supported input and output hardware devices.

    \author Brent Carrara
 */
#ifndef __ANDROID_CAHAL_DEVICE_H__
#define __ANDROID_CAHAL_DEVICE_H__

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <cpcommon.h>

#include "cahal.h"

#include "android_cahal.h"

/*! \var    android_callback_info
   \brief  Struct used to store buffer information required by the playback/
           record callbacks.
 */
typedef struct android_callback_info_t
{
  /*! \var    current_buffer_index
   *  \brief  Pointer to the current buffer used in the playback/record callback
   */
  UINT32  current_buffer_index;

  /*! \var    number_of_buffers
   *  \brief  The number of pointers in buffers.
   */
  UINT32  number_of_buffers;

  /*! \var    buffer_size
   *  \brief  The size in bytes in each buffer in buffers.
   */
  UINT32  buffer_size;

  /*! \var    buffers
   *  \brief  Buffers of audio data of size buffer_size.
   */
  UCHAR** buffers;

  /*! \var    context
   *  \brief  Pointers to data structures used in the process of recording/
   *          playback. These pointers are required when tearing down and
   *          stopping playback/record.
   */
  void*   context;

} android_callback_info;

/*! \var    android_recorder_context
 *  \brief  Platform-specific pointers to data structures required to record
 *          audio using the OpenSLES framework.
 */
typedef struct android_recoder_context_t
{
  /*! \var    audio_format
   *  \brief  Sample format used for recording, Pulse Code Modulation (PCM).
   */
  SLDataFormat_PCM*             audio_format;

  /*! \var    input_source
   *  \brief  The input source for recording (physical microphone)
   */
  SLDataSource*                 input_source;

  /*! \var    input_sink
   *  \brief  The sink for the recording (virtual queue)
   */
  SLDataSink*                   input_sink;

  /*! \var    recorder_object
   *  \brief  OpenSLES interface object used for recoding.
   */
  SLObjectItf                   recorder_object;

  /*! \var    recorder_interface
   *  \brief  Interface object for the recorder_object used to invoke commands
   *          to the API.
   */
  SLRecordItf                   recorder_interface;

  /*! \var    buffer_interface
   *  \brief  Interface to the audio queue used for recording.
   */
  SLAndroidSimpleBufferQueueItf buffer_interface;

} android_recorder_context;

/*! \var    android_playback_context
 *  \brief  Platform-specific pointers to data structures required to playbac
 *          audio using the OpenSLES framework.
 */
typedef struct android_playback_context_t
{
  /*! \var    audio_format
   *  \brief  Audio format used to encode samples for playback (Pulse Code
   *          Modulation (PCM)).
   */
  SLDataFormat_PCM*             audio_format;

  /*! \var    input_source
   *  \brief  The input source for playback (virtual queue)
   */
  SLDataSource*                 input_source;

  /*! \var    input_sink
   *  \brief  The input sink for playback (physical speaker)
   */
  SLDataSink*                   input_sink;

  /*! \var    playback_object
   *  \brief  Open SLES object used for playback.
   */
  SLObjectItf                   playback_object;

  /*! \var    playback_interface
   *  \brief  Interface to the playback_object, used to invoke API calls.
   */
  SLPlayItf                     playback_interface;

  /*! \var    buffer_interface
   *  \brief  Interface to the OpenSLES buffer used to queue samples for
   *          playback.
   */
  SLAndroidSimpleBufferQueueItf buffer_interface;

} android_playback_context;

/*! \fn     cpc_error_code android_save_playback_context  (
              SLDataFormat_PCM*             in_audio_format,
              SLDataSource*                 in_input_source,
              SLDataSink*                   in_input_sink,
              SLObjectItf                   in_playback_object,
              SLPlayItf                     in_playback_interface,
              SLAndroidSimpleBufferQueueItf in_buffer_interface
                                )
    \brief  Stores the platform-specific structs in the global playback
            struct.

    \param  in_audio_format The format of the audio being playback.
    \param  in_input_source The output source for the playback audio (queue)
    \param  in_input_sink The output sink for the playback audio (speaker)
    \param  in_playback_object The AudioPlaybackObject used to playback audio.
    \param  in_playback_interface The interface used to control the playback
                                  object.
    \param  in_buffer_interface Interface to the buffer queue used to get bytes
                                of data from the user application to the OS.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_save_playback_context  (
    SLDataFormat_PCM*             in_audio_format,
    SLDataSource*                 in_input_source,
    SLDataSink*                   in_input_sink,
    SLObjectItf                   in_playback_object,
    SLPlayItf                     in_playback_interface,
    SLAndroidSimpleBufferQueueItf in_buffer_interface
                                );

/*! \fn     cpc_error_code android_save_recording_context  (
              SLDataFormat_PCM*             in_audio_format,
              SLDataSource*                 in_input_source,
              SLDataSink*                   in_input_sink,
              SLObjectItf                   in_recorder_object,
              SLRecordItf                   in_recorder_interface,
              SLAndroidSimpleBufferQueueItf in_buffer_interface
                                )
    \brief  Stores the platform-specific structs in the global record
            struct.

    \param  in_audio_format The format of the audio being recorded.
    \param  in_input_source The output source for the recorded audio (mic)
    \param  in_input_sink The output sink for the recorded audio (queue)
    \param  in_recorder_object The AudioPlaybackObject used to record audio.
    \param  in_recorder_interface The interface used to control the record
                                  object.
    \param  in_buffer_interface Interface to the buffer queue used to get bytes
                                of data from the OS to the user application.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_save_recording_context  (
    SLDataFormat_PCM*             in_audio_format,
    SLDataSource*                 in_input_source,
    SLDataSink*                   in_input_sink,
    SLObjectItf                   in_recorder_object,
    SLRecordItf                   in_recorder_interface,
    SLAndroidSimpleBufferQueueItf in_buffer_interface
                                );

/*! \fn     void android_recorder_callback (
              SLAndroidSimpleBufferQueueItf in_recorder_buffer,
              void*                         in_user_data
                                    )
    \brief  Callback called when a full buffer of recorded sample is ready
            for processing. This is the interface for Android simple buffer
            queue callbacks.

    \param  in_recorder_buffer  The buffer used to record the samples.
    \param  in_user_data        The user data needed by the callback to grab
                                the buffer that was filled by the OS. The
                                user data contains a pointer to the buffer
                                that was filled.
 */
void
android_recorder_callback (
    SLAndroidSimpleBufferQueueItf in_recorder_buffer,
    void*                         in_user_data
                          );

/*! \fn     void android_playback_callback (
              SLAndroidSimpleBufferQueueItf in_playback_buffer,
              void*                         in_user_data
                                    )
    \brief  Callback called when a full buffer of samples is needed for
            playback. This is the interface for Android simple buffer
            queue callbacks.

    \param  in_playback_buffer  The buffer used to playback the samples.
    \param  in_user_data        The user data needed by the callback to grab
                                the buffer that is to be filled and enqueued in
                                the playback buffer.
 */
void
android_playback_callback (
    SLAndroidSimpleBufferQueueItf in_playback_buffer,
    void*                         in_user_data
                          );

/*! \fn     cpc_error_code android_initialize_playback_structs  (
              UINT32            in_number_of_channels,
              FLOAT64           in_sample_rate,
              UINT32            in_bit_depth,
              SLDataFormat_PCM* out_audio_format,
              SLDataSource*     out_output_source,
              SLDataSink*       out_output_sink
                                      )
    \brief  Helper function to fill the neccessary data structures required
            by the OpenSLES framework for playback.

    \param  in_number_of_channels The number of channels that the audio will be
                                  played back in.
    \param  in_sample_rate  The sample rate to play the audio back at.
    \param  in_bit_depth  The quantization level of the samples.
    \param  out_audio_format  The OpenSLES audio form structure that gets filled
    \param  out_output_source The OpenSLES output source (buffer)
    \param  out_output_sink The OpenSLES output sink (speaker)
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_initialize_playback_structs  (
    UINT32            in_number_of_channels,
    FLOAT64           in_sample_rate,
    UINT32            in_bit_depth,
    SLDataFormat_PCM* out_audio_format,
    SLDataSource*     out_output_source,
    SLDataSink*       out_output_sink
                                      );

/*! \fn     cpc_error_code android_initialize_recording_structs  (
              UINT32            in_number_of_channels,
              FLOAT64           in_sample_rate,
              UINT32            in_bit_depth,
              SLDataFormat_PCM* out_audio_format,
              SLDataSource*     out_input_source,
              SLDataSink*       out_input_sink
                                      )
    \brief  Helper function to fill the necessary data structures required
            by the OpenSLES framework for recording.

    \param  in_number_of_channels The number of channels that the audio will be
                                  recorded at.
    \param  in_sample_rate  The sample rate to record the audio at.
    \param  in_bit_depth  The quantization level of the samples.
    \param  out_audio_format  The OpenSLES audio form structure that gets filled
    \param  out_input_source The OpenSLES input source (mic)
    \param  out_input_sink The OpenSLES input sink (queue)
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_initialize_recording_structs  (
    UINT32            in_number_of_channels,
    FLOAT64           in_sample_rate,
    UINT32            in_bit_depth,
    SLDataFormat_PCM* out_audio_format,
    SLDataSource*     out_input_source,
    SLDataSink*       out_input_sink
                                      );

/*! \fn     cpc_error_code android_register_playback (
              cahal_device*                   in_device,
              FLOAT32                         in_volume,
              SLDataSource*                   in_output_source,
              SLDataSink*                     in_output_sink,
              SLObjectItf*                    io_playback_object,
              SLPlayItf*                      out_playback_interface,
              SLAndroidSimpleBufferQueueItf*  out_buffer_interface
                          )
    \brief  Configures the OpenSLES playback object, interface and output
            buffer queue.

    \param  in_device The device to playback audio to
    \param  in_volume The volume to playback at. Value is in the range [ 0, 1 ].
    \param  in_output_source  The OpenSLES output source (queue)
    \param  in_output_sink  The OpenSLES output sink (speaker)
    \param  io_playback_object  The configured OpenSLES playback object
    \param  out_playback_interface  The OpenSLES interface to the playback
                                    object.
    \param  out_buffer_interface  The OpenSLES interface to the playback
                                  buffer.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_register_playback (
    cahal_device*                   in_device,
    FLOAT32                         in_volume,
    SLDataSource*                   in_output_source,
    SLDataSink*                     in_output_sink,
    SLObjectItf*                    io_playback_object,
    SLPlayItf*                      out_playback_interface,
    SLAndroidSimpleBufferQueueItf*  out_buffer_interface
                          );

/*! \fn     cpc_error_code android_register_recorder (
              cahal_device*                   in_device,
              SLDataSource*                   in_input_source,
              SLDataSink*                     in_input_sink,
              SLObjectItf*                    io_recorder_object,
              SLRecordItf*                    out_recorder_interface,
              SLAndroidSimpleBufferQueueItf*  out_buffer_interface
                          )
    \brief  Configures the OpenSLES recorder object, interface and output
            buffer queue.

    \param  in_device The device to record audio from
    \param  in_input_source  The OpenSLES input source (mic)
    \param  in_input_sink  The OpenSLES input sink (queue)
    \param  io_recorder_object  The configured OpenSLES recorder object
    \param  out_recorder_interface  The OpenSLES interface to the recorder
                                    object.
    \param  out_buffer_interface  The OpenSLES interface to the recorder
                                  buffer.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_register_recorder (
    cahal_device*                   in_device,
    SLDataSource*                   in_input_source,
    SLDataSink*                     in_input_sink,
    SLObjectItf*                    io_recorder_object,
    SLRecordItf*                    out_recorder_interface,
    SLAndroidSimpleBufferQueueItf*  out_buffer_interface
                          );

/*! \fn     cpc_error_code android_register_playback_callback  (
              cahal_device*                 in_device,
              cahal_playback_callback       in_playback,
              void*                         in_callback_user_data,
              SLPlayItf                     in_playback_interface,
              SLAndroidSimpleBufferQueueItf in_buffer_interface,
              cahal_playback_info**         out_playback_callback_info
                                    )
    \brief  Registers a callback function to be called when more samples
            are required by the OS.

    \param  in_device The device to playback audio to
    \param  in_playback The user-defined callback function to call when samples
                        are required for playback.
    \param  in_callback_user_data The user-defined data to pass back to the
                                  callback (in_playback) when samples are
                                  required.
    \param  in_playback_interface The OpenSLES interface to the playback object
    \param  in_buffer_interface The OpenSLES interface to the playback object
    \param  out_playback_callback_info  Configured callback info struct that
                                        contains pointers to all OpenSLES
                                        structures.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_register_playback_callback  (
    cahal_device*                 in_device,
    cahal_playback_callback       in_playback,
    void*                         in_callback_user_data,
    SLPlayItf                     in_playback_interface,
    SLAndroidSimpleBufferQueueItf in_buffer_interface,
    cahal_playback_info**         out_playback_callback_info
                                    );

/*! \fn     cpc_error_code android_register_recorder_callback  (
              cahal_device*                 in_device,
              cahal_recorder_callback       in_recorder,
              void*                         in_callback_user_data,
              SLRecordItf                   in_recorder_interface,
              SLAndroidSimpleBufferQueueItf in_buffer_interface,
              cahal_recorder_info**         out_recorder_callback_info
                                              )
    \brief  Registers a callback function to be called when a buffer is full.

    \param  in_device The device to record audio from
    \param  in_recorder The user-defined callback function to call when a
                        buffer is full of audio samples.
    \param  in_callback_user_data The user-defined data to pass back to the
                                  callback (in_recorder) when samples are
                                  available.
    \param  in_recorder_interface The OpenSLES interface to the recorder object
    \param  in_buffer_interface The OpenSLES interface to the recorder buffer
    \param  out_recorder_callback_info  Configured callback info struct that
                                        contains pointers to all OpenSLES
                                        structures.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_register_recorder_callback  (
    cahal_device*                 in_device,
    cahal_recorder_callback       in_recorder,
    void*                         in_callback_user_data,
    SLRecordItf                   in_recorder_interface,
    SLAndroidSimpleBufferQueueItf in_buffer_interface,
    cahal_recorder_info**         out_recorder_callback_info
                                    );

/*! \fn     cpc_error_code android_compute_bytes_per_buffer  (
              SLDataFormat_PCM* in_audio_format,
              FLOAT32           in_number_of_seconds,
              UINT32*           out_bytes_per_buffer
                                  )
    \brief  Computes the number of bytes in an audio buffer passed on the
            audio format being used and the number of seconds of audio
            to buffer.

    \param  in_audio_format Structure defining the format of the audio being
                            processed.
    \param  in_number_of_seconds  The number of seconds of audio each buffer
                                  will contain.
    \param  out_bytes_per_buffer  The number of bytes required to store audio
                                  in in_audio_format for in_number_of_seconds.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_compute_bytes_per_buffer  (
    SLDataFormat_PCM* in_audio_format,
    FLOAT32           in_number_of_seconds,
    UINT32*           out_bytes_per_buffer
                                  );

/*! \fn     cpc_error_code android_enqueue_playback_buffers (
              SLDataFormat_PCM*             in_audio_format,
              SLAndroidSimpleBufferQueueItf in_buffer_interface,
              cahal_playback_info*          out_playback_callback_info
                                                            )
    \brief  Allocate, populate and enqueue buffers onto the OpenSLES buffer.

    \param  in_audio_format Format of the audio samples to play back.
    \param  in_buffer_interface The buffer to enqueue populated buffers of
                                 audio samples onto.
    \param  out_playback_callback_info  The callback context to add the newly
                                        created audio buffers to.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_enqueue_playback_buffers  (
    SLDataFormat_PCM*             in_audio_format,
    SLAndroidSimpleBufferQueueItf in_buffer_interface,
    cahal_playback_info*          out_playback_callback_info
                                );

/*! \fn     cpc_error_code android_enqueue_record_buffers (
              SLDataFormat_PCM*             in_audio_format,
              SLAndroidSimpleBufferQueueItf in_buffer_interface,
              cahal_recorder_info*          out_recorder_callback_info
                                                            )
    \brief  Allocate and enqueue buffers onto the OpenSLES buffer.

    \param  in_audio_format Format of the audio samples to be recorded.
    \param  in_buffer_interface The buffer to enqueue empty buffers onto.
    \param  out_recorder_callback_info  The callback context to add the newly
                                        created audio buffers to.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_enqueue_record_buffers  (
    SLDataFormat_PCM*             in_audio_format,
    SLAndroidSimpleBufferQueueItf in_buffer_interface,
    cahal_recorder_info*          out_recorder_callback_info
                                );

#endif /* __ANDROID_CAHAL_DEVICE_H__ */
