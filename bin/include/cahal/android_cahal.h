/*! \file   android_cahal.h
 *  \brief  Android platform specific functions to setup the OpenSLES framework.

    \author Brent Carrara
 */
#ifndef __ANDROID_CAHAL_H__
#define __ANDROID_CAHAL_H__

#include <unistd.h>
#include <stdlib.h>

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#include <cpcommon.h>

#include "cahal.h"

#include "android_cahal_audio_format_description.h"

/*! \def    ANDROID_DEVICE_HANDLE_OUTPUT
    \brief  The default device handle used to keep track of the output device.
 */
#define ANDROID_DEVICE_HANDLE_OUTPUT          1

/*! \def    ANDROID_DEVICE_STREAM_DEFAULT_HANDLE
 *  \brief  The stream handle to use as each input/output only has a single
 *          stream.
 */
#define ANDROID_DEVICE_STREAM_DEFAULT_HANDLE  1

/*! \var    g_engine_object
 *  \brief  The OpenSLES object that all other objects and interfaces are
 *          derived from.
 *
 */
extern SLObjectItf g_engine_object;

/*! \var    g_engine_interface
 *  \brief  The OpenSLES interface to the Android object.
 *
 */
extern SLEngineItf g_engine_interface;

/*! \fn     void android_terminate( void )
 *  \brief  Platform-specific terminate entry point that handles destroying
 *          the OpenSLES objects.
 *
 */
void
android_terminate( void );

/*! \fn     cpc_error_code android_create_output_source   (
              SLDataFormat_PCM* in_audio_format,
              SLDataSource*     out_output_source
                                                          )
    \brief  Generates a new output source (buffer queue)

    \param  in_audio_format The format of the audio being played back.
    \param  out_output_source A newly created/configured output source
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_create_output_source  (
    SLDataFormat_PCM* in_audio_format,
    SLDataSource*     out_output_source
                              );

/*! \fn     cpc_error_code android_create_input_source   (
              SLDataSource*     out_input_source
                                                         )
    \brief  Generates a new input source (mic)

    \param  out_input_source A newly created/configured input source
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_create_input_source (
    SLDataSource *out_input_source
                            );

/*! \fn     cpc_error_code android_create_output_sink  (
              SLDataSink* out_output_sink
                            )
    \brief  Generates a new output sink (speaker)

    \param  out_output_sink A newly created/configured output sink
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_create_output_sink  (
    SLDataSink* out_output_sink
                            );

/*! \fn     cpc_error_code android_create_input_sink (
              SLDataFormat_PCM* in_audio_format,
              SLDataSink*       out_input_sink
                          )
    \brief  Generates a new input sink (mic)

    \param  in_audio_format The format of the audio to be recorded in
    \param  out_input_sink  The newly created/configured iput sink.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_create_input_sink (
    SLDataFormat_PCM* in_audio_format,
    SLDataSink*       out_input_sink
                          );

/*! \fn     SLresult android_set_config  (
              SLObjectItf in_recorder_object,
              UINT32      in_configuration
                    )
    \brief  Sets the configuration on the OpenSLES record object. The
            configuration sets what mic is used in recording.

    \param  in_recorder_object  The object to set the configuration on.
    \param  in_configuration  The configuration to set.
    \return RESULT_SUCCES or an OpenSLES-specific error
 */
SLresult
android_set_config  (
    SLObjectItf in_recorder_object,
    UINT32      in_configuration
                    );

/*! \fn     CPC_BOOL android_test_input_configuration (
              SLDataFormat_PCM* in_audio_format,
              UINT32            in_configuration
                                )
    \brief  Test a specific configuration. Return true if the configuration
            defined in in_audio_format is supported by the hardware.

    \param  in_audio_format The audio format to test to see if its supported.
    \param  in_configuration  The input configuration to test.
    \return True iff the format/configuration is supported in hardware.
 */
CPC_BOOL
android_test_input_configuration (
    SLDataFormat_PCM* in_audio_format,
    UINT32            in_configuration
                                );

/*! \fn     CPC_BOOL android_test_output_configuration (
              SLDataFormat_PCM* in_audio_format
                                )
    \brief  Test a specific configuration. Return true if the configuration
            defined in in_audio_format is supported by the hardware.

    \param  in_audio_format The audio format to test to see if its supported.
    \return True iff the format/configuration is supported in hardware.
 */
CPC_BOOL
android_test_output_configuration (
    SLDataFormat_PCM* in_audio_format
                                  );

/*! \fn     cpc_error_code android_init_input_device_struct  (
              UINT32                in_configuration_index,
              cahal_device**        out_device,
              cahal_device_stream** out_stream
                                  )
    \brief  Create the structures required to define a cahal_device. Android
            has one stream per device.

    \param  in_configuration_index  The configuration set in the device struct.
                                    The configuration is used as the handle of
                                    the device.
    \param  out_device  The newly created cahal device.
    \param  out_stream  The newly created cahal device stream.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_init_input_device_struct  (
    UINT32                in_configuration_index,
    cahal_device**        out_device,
    cahal_device_stream** out_stream
                                  );

/*! \fn     cpc_error_code android_set_audio_format_struct (
              UINT32            in_num_channels,
              UINT32            in_sample_rate,
              UINT32            in_bits_per_sample,
              SLDataFormat_PCM* out_audio_format
                                )
    \brief  Create a new audio format struct with the given parameters.

    \param  in_num_channels The number of channels supported in the audio format
    \param  in_sample_rate  The sample rate set in the format
    \param  in_bits_per_sample  The bit depth of each sample (quantization
                                level)
    \param  out_audio_format  The newly created output audio format.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_set_audio_format_struct (
    UINT32            in_num_channels,
    UINT32            in_sample_rate,
    UINT32            in_bits_per_sample,
    SLDataFormat_PCM* out_audio_format
                                );

/*! \fn     CPC_BOOL android_init_and_test_output_configuration (
              UINT32 in_num_channels,
              UINT32 in_sample_rate,
              UINT32 in_bits_per_sample
                                            )
    \brief  Function that simply sets up a test configuraiton and tests
            to see if it is supported in hardware.

    \param  in_num_channels The number of channels in the format to test.
    \param  in_sample_rate  The sample rate to test
    \param  in_bits_per_sample  The bit depth (quantization) to test
    \return True iff the format is supported, false otherwise.
 */
CPC_BOOL
android_init_and_test_output_configuration (
    UINT32 in_num_channels,
    UINT32 in_sample_rate,
    UINT32 in_bits_per_sample
                                            );

/*! \fn     CPC_BOOL android_init_and_test_input_configuration (
              UINT32 in_num_channels,
              UINT32 in_sample_rate,
              UINT32 in_bits_per_sample,
              UINT32 in_configuration
                                            )
    \brief  Function that simply sets up a test input configuration and tests
            to see if it is supported in hardware.

    \param  in_num_channels The number of channels in the format to test.
    \param  in_sample_rate  The sample rate to test
    \param  in_bits_per_sample  The bit depth (quantization) to test
    \param  in_configuration  The configuration that is tested (id into the
                              input configuration array).
    \return True iff the format is supported, false otherwise.
 */
CPC_BOOL
android_init_and_test_input_configuration (
    UINT32 in_num_channels,
    UINT32 in_sample_rate,
    UINT32 in_bits_per_sample,
    UINT32 in_configuration
                                    );

/*! \fn     cpc_error_code android_set_output_device (
              cahal_device*** io_device_list,
              UINT32*         io_num_devices
                          )
    \brief  Collect the list of supported output audio configurations and adds
            them to the list.

    \param  io_device_list  The list to add any supported audio devices to.
    \param  io_num_devices The updated number of supported devices.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_set_output_device (
    cahal_device*** io_device_list,
    UINT32*         io_num_devices
                          );

/*! \fn     cpc_error_code android_set_input_devices (
              cahal_device*** io_device_list,
              UINT32*         io_num_devices
                          )
    \brief  Collect the list of supported input audio configurations and adds
            them to the list.

    \param  io_device_list  The list to add any supported audio devices to.
    \param  io_num_devices The updated number of supported devices.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_set_input_devices (
    cahal_device*** io_device_list,
    UINT32*         io_num_devices
                          );

/*! \fn     cpc_error_code android_add_device_to_list  (
              cahal_device*   in_device,
              cahal_device*** io_device_list,
              UINT32*         io_number_of_devices
                            )
    \brief  Helper function to add in_device to io_device_list. Updates the
            number of devices in io_number_of_devices.

    \note   io_device_list actually contains io_number_of_devices+1 with the +1
            being the null terminator.

    \param  in_device The device to add to the list.
    \param  io_device_list  The list to add devices to.
    \param  io_number_of_devices  The number of devices in io_device_list.
    \return NO_ERROR if the structures have been configured, an error code
            otherwise.
 */
cpc_error_code
android_add_device_to_list  (
    cahal_device*   in_device,
    cahal_device*** io_device_list,
    UINT32*         io_number_of_devices
                            );

#endif /* __ANDROID_CAHAL_H__ */
