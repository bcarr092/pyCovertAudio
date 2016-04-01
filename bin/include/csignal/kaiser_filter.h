/*! \file   kaiser_filter.h
    \brief  This is the header file for the finite impulse response (FIR) Kaiser
            filter struct and associated helper functions.
 
            A Kaiser filter is optimal in that it will generate an optimal
            (maximal attenuation) filter required to satisfy the design
            parameters of the filter. A step-by-stop guide to building a
            Kaiser function can be found in:
 
            Introduction to Signal Processing by Sophocles J. Orfanidis
            http://www.ece.rutgers.edu/~orfanidi/intro2sp/orfanidis-i2sp.pdf
 
            Where all other types of window functions have set transition ranges
            and attenuation factors given the number of taps in the filter, a
            Kaiser filter can be designed (i.e. number of taps derived, filter
            coefficients derived) based on the design parameters of the filter.
            A Kaiser filter is used when more control over the parameters is
            required.
 
    \author Brent Carrara
 */
#ifndef __KAISER_FILTER_H__
#define __KAISER_FILTER_H__

#include <cpcommon.h>

#include "fir_filter.h"
#include "csignal_error_codes.h"

/*! \fn     csignal_error_code csignal_initialize_kaiser_filter  (
              FLOAT32                in_first_stopband,
              FLOAT32                in_first_passband,
              FLOAT32                in_second_passband,
              FLOAT32                in_second_stopband,
              FLOAT32                in_passband_attenuation,
              FLOAT32                in_stopband_attenuation,
              UINT32                 in_sampling_frequency,
              fir_passband_filter**  out_filter
            )
    \brief  Initializesk a new fir_passband_filter struct as a Kaiser filter
            and designed according to the input parameters of the function.
            The passband region is between first and second passband, the
            stopband regions are between zero and first stopband and
            second stopband and sampling_frequency / 2. The coefficients of
            out_filter are set to an ideal filter windowed using a Kaiser window
 
    \param  in_first_stopband The upper range value in the first stopband. The
                              range of the first stopband is [0,first_stopband].
                              Units are Hz.
    \param  in_first_passband The lower range value in the passband. The
                              passband range is [first,second]. Units are Hz.
    \param  in_second_passband  The upper range value in the passband. The
                                passband range is [ first, second ]. Units are
                                Hz.
    \param  in_second_stopband  The lower range value in the second stopband.
                                The range of the second stopband is [second,
                                sampling_frequency/2]. Units are Hz.
    \param  in_passband_attenuation The amount of "ripple" in magnitude (dB),
                                    i.e deviation from 0 dB in the passband
                                    region. Units are dB.
    \param  in_stopband_attenuation The amount of attenuation in magnitude (dB)
                                    from the passband region to the stopband
                                    region. Units are dB.
    \param  in_sampling_frequency The sampling frequency used to generate the
                                  filter. Units are Hz.
    \param  out_filter  The newly constructed Kaiser Filter. If no errors are
                        generated this will be set to a fir_passband_filter
                        struct with coefficients set.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_filter is null.
            CPC_ERROR_CODE_INVALID_PARAMETER If any of the passband or stopband
                                              values are 0 or negative. If
                                              first stopband is greater than
                                              first passband. If second passband
                                              is greater than second stopband.
                                              If sampling_frequency is negative.
 */
csignal_error_code
csignal_initialize_kaiser_filter  (
                                 FLOAT32                in_first_stopband,
                                 FLOAT32                in_first_passband,
                                 FLOAT32                in_second_passband,
                                 FLOAT32                in_second_stopband,
                                 FLOAT32                in_passband_attenuation,
                                 FLOAT32                in_stopband_attenuation,
                                 UINT32                 in_sampling_frequency,
                                 fir_passband_filter**  out_filter
                                   );

/*! \fn     csignal_error_code csignal_inititalize_kaiser_lowpass_filter  (
              FLOAT32               in_passband,
              FLOAT32               in_stopband,
              FLOAT32               in_passband_attenuation,
              FLOAT32               in_stopband_attenuation,
              UINT32                in_sampling_frequency,
              fir_passband_filter** out_filter
            )
    \brief  Initializes a new fir_passband_filter struct as a LP Kaiser filter,
            designed according to the input parameters of the function.
            The passband region is between 0 and in_passband, the
            stopband region is between in_pasband and in_stopband. The
            coefficients of out_filter are set to an ideal filter windowed using
            a Kaiser window.
 
    \param  in_passband Frequencies between 0 and in_passband will be passed
                        through the filter with minor attenuation. Units are Hz.
    \param  in_stopband Frequencies above in_stopband will be attenuated
                        according to the in_stopband_attenuation value. Units
                        are in Hz.
    \param  in_passband_attenuation The amount of "ripple" in magnitude (dB),
                                    i.e deviation from 0 dB in the passband
                                    region. Units are dB.
    \param  in_stopband_attenuation The amount of attenuation in magnitude (dB)
                                    from the passband region to the stopband
                                    region. Units are dB.
    \param  in_sampling_frequency The sampling frequency used to generate the
                                  filter. Units are Hz.
    \param  out_filter  The newly constructed Kaiser low pass filter. If no 
                        errors are generated this will be set to a
                        fir_passband_filter struct with coefficients set.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_filter is null.
            CPC_ERROR_CODE_INVALID_PARAMETER If any of the passband or stopband
                                              values are 0 or negative. If
                                              first stopband is greater than
                                              first passband. If second passband
                                              is greater than second stopband.
                                              If sampling_frequency is negative.
 */
csignal_error_code
csignal_inititalize_kaiser_lowpass_filter(
                                  FLOAT32               in_passband,
                                  FLOAT32               in_stopband,
                                  FLOAT32               in_passband_attenuation,
                                  FLOAT32               in_stopband_attenuation,
                                  UINT32                in_sampling_frequency,
                                  fir_passband_filter** out_filter
                                          );

#endif  /*  __KAISER_FILTER_H__  */
