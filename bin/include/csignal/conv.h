/*! \file   conv.h
    \brief  Header file for convolution of signals.
 */
#ifndef __CONV_H__
#define __CONV_H__

#include <cpcommon.h>

#include "csignal_error_codes.h"

/*! \fn     csignal_error_code convolve  (
             USIZE       in_signal_one_length,
             FLOAT64*    in_signal_one,
             USIZE       in_signal_two_length,
             FLOAT64*    in_signal_two,
             USIZE*      out_signal_length,
             FLOAT64**   out_signal
            )
    \brief  Perfors the convolution of signal one and two, stores the output
            in a newly created array, out_signal. Performs the operation
            out_signal = in_signal_one * in_signal_two (where * is convolution).
 
    \note   If out_signal_length is non-zero and out_signal is non_Null then
            out_signal is created by this function and returned. It must be
            freed by the caller.
 
    \param  in_signal_one_length  The number of elements in signal one.
    \param  in_signal_one One of the two signals to be convolved.
    \param  in_signal_two_length  The number of elements in siganl two.
    \param  in_signal_two The second signal to be used in the convolution.
    \param  out_signal_length The number of elements in out_signal, 0 if an 
                              error occurrs. If successful this will equal
                              signal one length + signal two length.
    \param  out_signal  A newly created array containing the values of the
                        convolution between signal one and two.
    \return Returns NO_ERROR upon succesful exection or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If any of the input parameters are null.
 */
csignal_error_code
convolve  (
           USIZE       in_signal_one_length,
           FLOAT64*    in_signal_one,
           USIZE       in_signal_two_length,
           FLOAT64*    in_signal_two,
           USIZE*      out_signal_length,
           FLOAT64**   out_signal
           );

#endif  /*  __CONV_H__  */
