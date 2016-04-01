/*! \file   fft.h
    \brief  This file contains the headers for to calculate the FFT of a real-
            valued signal. The algorightm to calculate the FFT is from the
            following reference:

            Numerical recipes in C: the art of scientific computing by Press, et
            al. (Chapter 6)
            http://www2.units.it/ipl/students_area/imm2/files/Numerical_Recipes.pdf
 
    \author Brent Carrara
 */
#ifndef __FFT_H__
#define __FFT_H__

#include <cpcommon.h>

#include "csignal_error_codes.h"

/*! \fn     USIZE csignal_calculate_closest_power_of_two  (
              USIZE in_number
            )
    \brief  Finds the next power of 2 larger than or equal to in_number.
 
    \param  in_number The next power of 2 will be larger than or equal to this
                      parameter.
    \return A power of 2 larger than or equal to in_number.
 */
USIZE
csignal_calculate_closest_power_of_two  (
                                         USIZE in_number
                                         );

/*! \fn     csignal_error_code csignal_calculate_FFT (
              USIZE      in_signal_length,
              FLOAT64*   in_signal,
              USIZE*     out_fft_length,
              FLOAT64**  out_fft
            )
    \brief  Calculates the FFT of the input signal, in_signal.
 
    \note   If out_fft_length is non-zero and out_fft is non-Null, then no
            buffer will be allocated by this function. Otherwise, the caller
            needs to free out_fft.
 
    \param  in_signal_length  The number of elements in in_signal.
    \param  in_signal The signal whose FFT is to be calculated.
    \param  out_fft_length  The number of elements returned in out_fft. This
                            will always be 2 times the power of 2 larger than or
                            equal to in_signal_length. It is 2x larger because
                            a real and imaginary component are returned in
                            adjacent indices in out_fft.
    \param  out_fft The FFT of in_signal. The positive frequency components are
                    returned first starting with DC f=0Hz all the way up to the
                    sampling rate that in_signal was generated at divided by 2.
                    Then the negative freqeuncy components follow.
    \return Returns NO_ERROR upon succesful exection or one of these errors
            (see cpc_safe_malloc for more error codes):
 
            CPC_ERROR_CODE_NULL_POINTER If in_signal, out_signal_lenght or
                                        out_signal are null.
 */
csignal_error_code
csignal_calculate_FFT (
                       USIZE      in_signal_length,
                       FLOAT64*   in_signal,
                       USIZE*     out_fft_length,
                       FLOAT64**  out_fft
                       );

csignal_error_code
csignal_calculate_IFFT (
                       USIZE      in_fft_length,
                       FLOAT64*   in_fft,
                       USIZE*     out_signal_length,
                       FLOAT64**  out_signal
                       );


#endif  /*  __FFT_H__ */
