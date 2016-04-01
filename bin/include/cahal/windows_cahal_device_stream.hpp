/*! \file   windows_cahal_device_stream.hpp
    \brief  Function prototypes for functions related to querying the
            underlying hardware for supported device streams.

    \author Brent Carrara
 */

#ifndef __WINDOWS_CAHAL_DEVICE_STREAM_H__
#define __WINDOWS_CAHAL_DEVICE_STREAM_H__

#include <mmdeviceapi.h>

#include <Audioclient.h>

#include "cahal.h"
#include "cahal_device.h"
#include "cahal_device_stream.h"

/*! \def    WINDOWS_DEVICE_STREAM_DEFAULT_HANDLE
    \brief  Handle to use for all streams. In Windows devices are either an
            input or output device.
 */
#define WINDOWS_DEVICE_STREAM_DEFAULT_HANDLE  1

/*! \fn     HRESULT windows_set_device_streams(
              cahal_device* out_device,
              IMMDevice*    in_endpoint
            );
    \brief  Mallocs a single stream for each device, sets the stream's info
            then sets the supported formats for the stream.

    \param  out_device  The device whos stream is to be created and populated.
    \param  in_endpoint Pointer to the underlying audio hardware device.
    \return true iff no stream is added, or the stream was added successfully.
 */
HRESULT
windows_set_device_streams(
  cahal_device* out_device,
  IMMDevice*    in_endpoint
);

#endif  /*  __WINDOWS_CAHAL_DEVICE_STREAM_H__ */
