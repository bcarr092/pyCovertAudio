/*! \file   wav.h
    \brief  This library contains methods to read and write WAV files
 
    \author Brent Carrara
 */
#ifndef __WAV_H__
#define __WAV_H__

#include <cpcommon.h>

#include "csignal_error_codes.h"

/*! \def    CSIGNAL_RIFF_HEADER_ID
    \brief  Header string used to indicate the WAVE file is in RIFF (resource
            interchange file format)
 */
#define CSIGNAL_RIFF_HEADER_ID                 "RIFF"

/*! \def    CSIGNAL_RIFF_FILE_SIZE_BASE
    \brief  The base size of the RIFF file format header (for LPCM WAV)
 */
#define CSIGNAL_RIFF_FILE_SIZE_BASE            36

/*! \def    CSIGNAL_WAVE_HEADER_ID
    \brief  Header string used to indicate the RIFF file is a WAVE file.
 */
#define CSIGNAL_WAVE_HEADER_ID                 "WAVE"

/*! \def    CSIGNAL_WAVE_HEADER_CHUNK_FORMAT_ID
    \brief  Header string used to denote the start of the format header for
            the WAVE data
 */
#define CSIGNAL_WAVE_HEADER_CHUNK_FORMAT_ID    "fmt "

/*! \def    CSIGNAL_WAVE_HEADER_CHUNK_LPCM_FORMAT_SIZE
    \brief  The size of the format header when the data samples are ints
 */
#define CSIGNAL_WAVE_HEADER_CHUNK_LPCM_FORMAT_SIZE  16

/*! \def    CSIGNAL_WAVE_HEADER_CHUNK_FLOAT_FORMAT_SIZE
    \brief  The size of the format header when the data samples are floats
*/
#define CSIGNAL_WAVE_HEADER_CHUNK_FLOAT_FORMAT_SIZE  18

/*! \def    CSIGNAL_WAVE_HEADER_CHUNK_DATA_ID
    \brief  Header string used to denote the start of the data header for the
            WAVE data
 */
#define CSIGNAL_WAVE_HEADER_CHUNK_DATA_ID      "data"

/*! \def    CSIGNAL_WAVE_HEADER_FACT_ID
    \brief  Header string used to denote the start of the fact header used
            by non-PCM formats.
 */
#define CSIGNAL_WAVE_HEADER_FACT_ID            "fact"

/*!   \def    CSIGNAL_WAVE_HEADER_FACT_CHUNK_SIZE
      \brief  The size of the fact chunk.
 */
#define CSIGNAL_WAVE_HEADER_FACT_CHUNK_SIZE    4

/*! \def    CSIGNAL_WAVE_LPCM_FORMAT_CODE
    \brief  Code to indicate the WAV file is using LPCM encoding
 */
#define CSIGNAL_WAVE_LPCM_FORMAT_CODE           0x0001

/*! \def    CSIGNAL_WAVE_FLOAT_FORMAT_CODE
    \brief  Code to indicate the WAV file is using FLOAT encoding
 */
#define CSIGNAL_WAVE_FLOAT_FORMAT_CODE          0x0003

/*! \def    CSIGNAL_SHORT_SAMPLE_SIZE
    \brief  The size of each sample (in bytes)
 */
#define CSIGNAL_SHORT_SAMPLE_SIZE               2

/*! \def    CSIGNAL_FLOAT_SAMPLE_SIZE
    \brief  The size of each sample (in bytes)
 */
#define CSIGNAL_FLOAT_SAMPLE_SIZE               4

/*! \fn     csignal_error_code csignal_write_LPCM_wav  (
              CHAR*     in_file_name,
              USIZE     in_number_of_channels,
              UINT32    in_sample_rate,
              USIZE     in_number_of_samples,
              FLOAT64** in_samples
            )
    \brief  Main entry point to create a WAV file. Will write the data stored 
            in in_samples to a file named in_file_name. The WAV file will have
            in_number_of_channels channels, be sampled at in_sample_rate and
            each data stream (one per channel) will have in_number_of_samples.
            The in_samples array will be a two dimensional array, the first
            dimension will be the number of channels (should be equal in length
            to in_number_of_channels) and the second dimension will be samples
            (should be equal in length to in_number_of_samples).
 
    \param  in_file_name  The file name to use when creating the WAV file. This
                          function will fail if the file already exists.
    \param  in_number_of_channels The length of in_samples. There must be a
                                  data stream for each channel. Must be smaller
                                  than or equal to a UINT16.
    \param  in_sample_rate  The sample rate that the samples were generated at.
    \param  in_number_of_samples  The number of samples in each channel buffer.
                                  Must be smaller than or equal to a UINT32.
    \param  in_samples  The samples array it is an in_number_of_channels X
                        in_samples data buffer.
    \return Returns NO_ERROR upon succesful execution or one of these errors:

            CPC_ERROR_CODE_INVALID_PARAMETER  If a file with the name
                                              in_file_name already exists or
                                              could not be created. If the
                                              number of channels or the number
                                              of samples is larger than a UINT16
                                              or UINT32, respectively.
            CSIGNAL_ERROR_CODE_WRITE_ERROR  If data could not be written to
                                            the WAV file.
 */
csignal_error_code
csignal_write_LPCM_wav  (
                         CHAR*      in_file_name,
                         USIZE      in_number_of_channels,
                         UINT32     in_sample_rate,
                         USIZE      in_number_of_samples,
                         FLOAT64**  in_samples
                         );

/*! \fn     csignal_error_code csignal_write_FLOAT_wav  (
              CHAR*     in_file_name,
              USIZE     in_number_of_channels,
              UINT32    in_sample_rate,
              USIZE     in_number_of_samples,
              FLOAT64** in_samples
            )
    \brief  Main entry point to create a WAV file. Will write the data stored
            in in_samples to a file named in_file_name. The WAV file will have
            in_number_of_channels channels, be sampled at in_sample_rate and
            each data stream (one per channel) will have in_number_of_samples.
            The in_samples array will be a two dimensional array, the first
            dimension will be the number of channels (should be equal in length
            to in_number_of_channels) and the second dimension will be samples
            (should be equal in length to in_number_of_samples).
 
    \note   The WAV format expects float values between -1.0 and +1.0. This
            function DOES NOT check that the samples in in_samples adhere to
            this constraint (for reasons of performance). It is up to the caller
            to ensure this constraint is respected.

    \param  in_file_name  The file name to use when creating the WAV file. This
                          function will fail if the file already exists.
    \param  in_number_of_channels The length of in_samples. There must be a
                                  data stream for each channel. Must be smaller
                                  than or equal to a FLOAT64.
    \param  in_sample_rate  The sample rate that the samples were generated at.
    \param  in_number_of_samples  The number of samples in each channel buffer.
                                  Must be smaller than or equal to a UINT32.
    \param  in_samples  The samples array it is an in_number_of_channels X
                        in_samples data buffer.
    \return  Returns NO_ERROR upon succesful execution or one of these errors:

            CPC_ERROR_CODE_INVALID_PARAMETER  If a file with the name
                                              in_file_name already exists or
                                              could not be created. If the
                                              number of channels or the number
                                              of samples is larger than a UINT16
                                              or UINT32, respectively.
            CSIGNAL_ERROR_CODE_WRITE_ERROR  If data could not be written to
                                    the WAV file.
    */
csignal_error_code
csignal_write_FLOAT_wav (
                         CHAR*      in_file_name,
                         USIZE      in_number_of_channels,
                         UINT32     in_sample_rate,
                         USIZE      in_number_of_samples,
                         FLOAT64**  in_samples
                         );

#endif  /*  __WAV_H__  */
