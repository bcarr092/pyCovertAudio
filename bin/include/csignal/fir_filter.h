/*! \file   fir_filter.h
    \brief  This is the header file for the finite impulse response (FIR) filter
            struct and associated helper functions.

    \note   A FIR filter is used because we need to preserve the phase of the
            signal being filtered, i.e. we assume that the signal being
            filtered requires preservation of the phase of the signal and the
            only tolerable modification is a delay.
 
    \author Brent Carrara
 */
#ifndef __FIR_FILTER_H__
#define __FIR_FILTER_H__

#include <cpcommon.h>

#include "conv.h"

#include "csignal_error_codes.h"

/*! \var    fir_passband_filter
    \brief  This is the struct for a passband filter. It contains the relevant
            information regarding the filter, i.e. passband info and sampling
            frequency, and also contains the actual filter tap weights.
 
            A passband filter is designed to allow filter components within the
            passband (i.e. between first and second passband) through the filter
            (largely) unmodified while heavily attenuating frequency components
            outside the passband. In general, a passband filter is composed of a
            passband region, transition regions on either side of the passband
            (i.e. the frequency ranges where the filter goes from little to
            severe attenuation) and the stopband regions (i.e. the frequency
            ranges outside the passband and transition bands where the frequency
            components are attenuated the most severly).
 
            In theory, a perfect filter would be constructed as follows H(f) = 1
            for all frequency components, f, within the passband, and H(f) = 0
            for all frequency components, f, outside the passband.
            Unfortunately, when this frequency-domain function is converted to
            the time domain it results in a sync pulse which is infinitely long
            (and extends in both the +ve and -ve directions) creating a
            non-causal filter that requires an infinite number of points to
            represent. To account for this in practice the time domain function
            is windowed to time-bound the signal while ensuring that the filter
            continues to satisfy its design parameters. A good number of
            different windowing functions exist to perform this task:
            rectangular, triangular, Hann, Hamming, Bartlett-Hannnig, Kaiser,
            etc. Some good references for filter design are as follows:
 
            1)  Digital Filter Design by Zoran Milivojević
                http://www.mikroe.com/products/view/268/digital-filter-design/
            2)  Introduction to Signal Processing by Sophocles J. Orfanidis
                http://www.ece.rutgers.edu/~orfanidi/intro2sp/orfanidis-i2sp.pdf
 
            A filter is usually modelled as an impulse response consisiting of
            a number of 'taps.' This struct models the impulse response in an
            array of taps which are the impulse response values. We model the
            filter in this way so that it can easily be applied to time-domain
            signals by performing convolution.
 */
typedef struct fir_passband_filter_t
{
  /*! \var    first_passband
      \brief  This the low frequency component for the passband range, i.e.
              the passband (frequency components that are unmodified) is between
              first_passband and second_passband.
   */
  FLOAT32   first_passband;
  
  /*! \var    second_passband
      \brief  This is the high component for the passband range, i.e.
              the passband (frequency components that are unmodified) is between
              first_passband and second_passband.
   */
  FLOAT32   second_passband;
  
  /*! \var    sampling_frequency
      \brief  The sampling rate is 1 / sampling_frequency, which is the inter-
              sample time (i.e. delay) between coefficients of the filter. This
              struct models the impulse response of the filter and therefore
              the duration (or delay) of the filter is 1 / sampling_frequency *
              number_of_taps.
   */
  UINT32    sampling_frequency;
  
  /*! \var    number_of_taps
      \brief  The number of taps, or samples in the impulse response.
   */
  USIZE     number_of_taps;
  
  /*! \var    coefficients
      \brief  The values of the impulse response.
   */
  FLOAT64*  coefficients;
  
} fir_passband_filter;

/*! \fn     csignal_error_code csignal_initialize_passband_filter (
              FLOAT32               in_first_passband,
              FLOAT32               in_second_passband,
              UINT32                in_sampling_frequency,
              USIZE                 in_number_of_taps,
              fir_passband_filter*  out_filter
            )
    \brief  Initializes a fir_passband_filter struct with the passed in values.
            Mallocs enough FLOAT64 values in the coefficients member to hold
            in_number_of_taps values and initializes them to zero. Validates
            the input parameters as well.
 
    \param  in_first_passband The first frequency component in the passband
                              range. Must be strictly less than second_passband.
                              Units are Hz.
    \param  in_second_passband  The second frequency component in the passband
                                range. Must be strictly greater than
                                first_passband. Units are Hz.
    \param  in_sampling_frequency The sampling frequency used to generate the
                                  filter. Units are Hz.
    \param  in_number_of_taps The number of taps in the filter. This is the
                              number of coefficients used for the filter.
    \param  out_filter  The populated filter struct with the coefficients array
                        malloc'd and initialized to zero.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_filter is null.
            CPC_ERROR_CODE_INVALID_PARAMETER If first >= second, first or second
                                              are zero, or the number of taps or
                                              sampling frequency are zero.
 */
csignal_error_code
csignal_initialize_passband_filter (
                                    FLOAT32               in_first_passband,
                                    FLOAT32               in_second_passband,
                                    UINT32                in_sampling_frequency,
                                    USIZE                 in_number_of_taps,
                                    fir_passband_filter*  out_filter
                                    );

/*! \fn     csignal_error_code csignal_destroy_passband_filter(
              fir_passband_filter* io_filter
            )
    \brief  Safely frees the passband filter struct. If the coefficients array
            has been malloc'd it will be freed as well.
 
    \param  io_filter The filter structure to be freed.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_free for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If io_filter is null.
 */
csignal_error_code
csignal_destroy_passband_filter(
                                fir_passband_filter* io_filter
                                );

/*! \fn     csignal_error_code csignal_filter_signal (
              fir_passband_filter* in_filter,
              USIZE                in_signal_length,
              FLOAT64*             in_signal,
              USIZE*               out_filtered_signal_length,
              FLOAT64**            out_filtered_signal
            )
    \brief  Performs a convolution of the coefficients in in_filter and the
            signal values in in_signal. Stores the results in
            out_filtered_signal. If an error occurrs out_filtered_signal_length
            will be set to 0 and out_filtered_signal will be set to NULL.
 
    \param  in_filter The filter to apply to in_signal.
    \param  in_signal_length  The number of samples in in_signal.
    \param  in_signal The samples to filter.
    \param  out_filtered_signal_length  The length of the filtered signal. Since
                                        convolution is performeed this will be
                                        in_signal_length + the number of taps in
                                        in_filter (if no error occurs).
    \param  out_filtered_signal The filtered signal. Note that if over or under
                                flow occur in filtering the signal the max/min
                                INT16 value will be used as the sample value.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_free for other possible errors):

            CPC_ERROR_CODE_NULL_POINTER If in_filer, in_signal, 
                                        out_filtered_signal_length or 
                                        out_filtered_signal are null.
            CPC_ERROR_CODE_INVALID_PARAMETER  If in_signal_length is 0 or the
                                              number of taps in in_filter is 0.
 */
csignal_error_code
csignal_filter_signal (
                       fir_passband_filter* in_filter,
                       USIZE                in_signal_length,
                       FLOAT64*             in_signal,
                       USIZE*               out_filtered_signal_length,
                       FLOAT64**            out_filtered_signal
                       );

/*! \fn     csignal_error_code csignal_filter_get_group_delay  (
              fir_passband_filter* in_filter,
              UINT32*              out_group_delay
            )
    \brief  Returns the number of samples of delay introduced by the use of the
            filter defined in in_filter.
 
    \note   This function ONLY works for linear phase filters (i.e., filters
            that delay all frequencies by the same amount). The underlying
            assumption is that the filter being passed as in_filter is linear
            phase, if that is not the case than a proper frequency-dependant
            group delay would need to be calculated.
 
    \note   This function does not perform any checks to ensure that in_filter
            is linear phase (for expediency - calculation requires a couple
            multiplications and a couple of FFTs).
 
    \param  in_filter The filter whose group delay is to be calculated.
    \param  out_group_delay The group delay (in samples) of the filter.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_free for other possible errors):

            CPC_ERROR_CODE_NULL_POINTER If any of the parameters are null.
            CPC_ERROR_INVALID_PARAMTER  If the number of taps in in_filter are
                                        not odd.
 */
csignal_error_code
csignal_filter_get_group_delay  (
                                 fir_passband_filter* in_filter,
                                 UINT32*              out_group_delay
                                 );

csignal_error_code
csignal_filter_get_filter_length  (
                                   fir_passband_filter*  in_filter,
                                   UINT32* in_filter_length
                                   );

#endif  /*  __FIR_FILTER_H__  */
