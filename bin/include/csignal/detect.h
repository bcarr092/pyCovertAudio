/*! \file   detect.h
    \brief  Functions used to detect the presence of input (i.e., received)
            signals.
 */
#ifndef __DETECT_H__
#define __DETECT_H__

#include <cpcommon.h>

#include "csignal.h"
#include "fft.h"
#include "fir_filter.h"
#include "gold_code.h"

#include "csignal_error_codes.h"

/*! \fn     csignal_error_code detect_calculate_energy (
              USIZE                in_signal_length,
              FLOAT64*             in_signal,
              USIZE                in_spread_signal_length,
              FLOAT64*             in_spread_signal,
              fir_passband_filter* in_narrowband_filter,
              fir_passband_filter* in_lowpass_filter,
              FLOAT64*             out_energy
            )
    \brief  Calculates the energy in in_signal by executing the following
            algorithm:
 
            a = multiply( in_signal, in_spread_signal )
            b = filter( narrowband_filter, a )
            c = multiply( b, b )
            d = filter( lowpass_filter, c )
            energy = sum( d )
 
    \param  in_signal_length  The number of elements in in_signal.
    \param  in_signal The signal whose energy is to be calculated.
    \param  in_spread_signal_length The number of elements in in_spread_signal.
    \param  in_spread_signal  The signal used to despread in_signal.
    \param  in_narrowband_filter  A BPF applied after in_signal is despread.
    \param  in_lowpass_filter A LPF applied to remove the double frequency
                              term created after squaring the signal.
    \param  out_energy  The energy in in_signal or NULL if an error occurrs.
    \return Returns NO_ERROR upon succesful exection or one of these errors:
 
            CPC_ERROR_CODE_NULL_POINTER If any of the input parameters are null.
 */
csignal_error_code
detect_calculate_energy (
                         USIZE                in_signal_length,
                         FLOAT64*             in_signal,
                         USIZE                in_spread_signal_length,
                         FLOAT64*             in_spread_signal,
                         fir_passband_filter* in_narrowband_filter,
                         fir_passband_filter* in_lowpass_filter,
                         FLOAT64*             out_energy
                         );

/*! \fn     csignal_error_code  detect_find_highest_energy_offset (
              USIZE                in_signal_length,
              FLOAT64*             in_signal,
              USIZE                in_spreading_signal_length,
              FLOAT64*             in_spread_signal,
              USIZE                in_number_of_tests,
              USIZE                in_step_size,
              fir_passband_filter* in_narrowband_filter,
              fir_passband_filter* in_lowpass_filter,
              FLOAT64              in_threshold,
              FLOAT64              in_exhaustive_difference,
              UINT32               in_exhaustive_decimation,
              USIZE*               out_offset
            )
    \brief  This function performs the following algorithm:
 
            1)  Checks in_signal[ i ] for energy above threshold, where 0 <= i
                <= in_number_of_tests and i jumps in_step_size between checks.
            2)  Ranges where the energy is above threshold are created from the
                results in step 1.
            3)  A hill climbing algorithm is performed on each range.
            4)  Once the hill climbing algorithm gets to a point where the energy
                on both sides of the hill are within in_exhaustive_difference
                percentage of each other the algorithm exhaustively checks
                the indices for a peak.
 
    \param  in_signal_length  The number of elements in in_signal.
    \param  in_signal The signal whose energy is to be calculated.
    \param  in_spreading_signal_length The number of elements in in_spread_signal.
    \param  in_spread_signal  The signal used to despread in_signal.
    \param  in_number_of_tests  The number of offsets to check in in_signal,
                                i.e., offsets 0 to in_number_of_tests in
                                in_signal are checked, during the phase algorithm
                                that searches for the beginning and end of hills.
    \param  in_step_size  The number of offsets (smaples) to skip between
                          each test (1 for exhaustive).
    \param  in_narrowband_filter  A BPF applied after in_signal is despread.
    \param  in_lowpass_filter A LPF applied to remove the double frequency
                              term created after squaring the signal.
    \param  in_exhaustive_difference  When performing the hill climbing
                                      algorithm the search strategy will switch
                                      to exhaustive search when the energy on
                                      both sides of the hill are within this
                                      difference (note that this is a percentage
                                      difference).
    \param  in_exhaustive_decimation  The decimation factor (step size) to use
                                      when the algorithm enters the exhaustive
                                      search mode.
    \param  in_threshold  When performing the initial phase of the algorithm that
                          looks for hills evidence of a hill is determined if
                          the energy at the specific offset is above in_threshold.
    \param  out_offset  The offset in in_signal with the highest energy.
    \return Returns NO_ERROR upon succesful exection or one of these errors:
 
            CPC_ERROR_CODE_NULL_POINTER If any of the input parameters are null.
            CPC_ERROR_CODE_INVALID_PARAMETER  If in_spread_signal_length >
                                              in_signal_length.
            CSIGNAL_ERROR_CODE_NO_RESULT  If no evidence of the spreading signal
                                          is found in in_signal (i.e., no offset
                                          in in_signal is above in_threshold).
 */
csignal_error_code
detect_find_highest_energy_offset (
                               USIZE                in_signal_length,
                               FLOAT64*             in_signal,
                               USIZE                in_spreading_signal_length,
                               FLOAT64*             in_spread_signal,
                               USIZE                in_number_of_tests,
                               USIZE                in_step_size,
                               fir_passband_filter* in_narrowband_filter,
                               fir_passband_filter* in_lowpass_filter,
                               FLOAT64              in_threshold,
                               FLOAT64              in_exhaustive_difference,
                               UINT32               in_exhaustive_decimation,
                               USIZE*               out_offset
                                   );

#endif  /*  __DETECT_H__  */
