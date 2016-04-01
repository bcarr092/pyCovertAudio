/*! \file   spreading_code.h
    \brief  This file contains all functions related to creating a spreading
            code (i.e. the c(t) function in spread spectrum modulation). Gold
            Codes are employed to allow for multiple access.
 
    \author Brent Carrara
 */
#ifndef __SPREADING_CODE_H__
#define __SPREADING_CODE_H__

#include <cpcommon.h>

#include "csignal_error_codes.h"

/*! \def    SPREADING_WAVEFORM_POSITIVE
    \brief  Float value used to represent a code chip value of 1 when expanding
            the spreading binary sequence into a sampled waveform.
 */
#define SPREADING_WAVEFORM_POSITIVE 1.0

/*! \def    SPREADING_WAVEFORM_NEGATIVE
    \brief  Float value used to represent a code chip value of 0 when expanding
            the spreading binary sequence into a sampled waveform.
 */
#define SPREADING_WAVEFORM_NEGATIVE -1.0

/*! \var    spreading_code
    \brief  Spreading codes are generated using a linear feedback shift register
            (LFSR) and should be used to generate m-sequences. That is maximum
            length sequences. A maximum length sequence will generate a code
            of length N = 2^n - 1 before repeating, where n is the number of
            stages in the LFSR (degree). This library can model LFSRs from 
            degree 2 to 32.
            
            A LFSR is composed of stages (think flip-flops) and taps. A tap can
            be opened or closed. A '1' bit in the generator_polynomial indicates
            the tap is closed in that bit position. Each time a bit is generated
            by the LFSR all bits in the state vector are shifted and all bits
            that have closed taps associated with them are summed modulo-2 to
            determine the next input bit. The output bit of the spreading code 
            generator is the right-most bit in the state variable.
 
            For an introductory discussion on LFSRs, m-sequences and Gold Codes
            see 'Introduction to Spread Spectrum Communications' by Peterson,
            et al., Chapter 3.
 
            In our modelling of LSFR we store the generator polynomial as
            follows. If r is the degree of the polynomial there are r+1 taps:
            g_r, g_{r-1}, ..., g_0. A valid LFSR must have g_r = g_0 = 1 and
            we therefore do not model the g_r bit (since it is not included in
            any calculations to determine the next input bit). We store the tap
            coefficients (i.e. closed = 1, open = 0) as a bit-mask using a
            UINT32. The coefficients are stored starting at the MSB always. That
            is a generator polynomial is stored as follows:
 
            g_{r-1} g_{r-2} g_{r-3} .... g_0 0 0 0 .... 0
 
            where there 32 - r degree trailing zeros in the LSB bit-positions. As
            an example a r = 2 LFSR with a generator that has all taps closed
            would be:
 
            1100 0000 0000 0000 0000 0000 0000 0000
 
            We, similarly, model the initial state in the same way.
 */
typedef struct spreading_code_t
{
  /*! \var    degree
      \brief  The degree of the polynomial used to generate code sequences using
              the LFSR. This library supports degrees between 2 and 32
              (inclusive).
   */
  UCHAR degree;
  
  /*! \var    generator_polynomial
      \brief  The generator polynomial must be a primitive polynomial to ensure
              an m-sequence is generated. The generator polynomial has a '1'
              bit set if there is a closed tap in that position in the LFSR. If
              a 4 stage LFSR was being used with the generator polynomial bit
              vector [ 1010 0000 0000 0000 0000 0000 0000 0000 ] it would
              indicate that the 2nd and 4th taps are closed.
   
      \note   The LSB in the polynomial MUST BE '1' (that is the bit in the
              32 - degree position). The MSB is implicitly '1'.
   */
  UINT32 generator_polynomial;
  
  /*! \var    state
      \brief  This is the current state of the LFSR, i.e. the configuration of
              all the stages. Each bit in the code in the sequence that is
              generated bit-shifts the state variable 1 position to the right 
              and unneeded bits are masked away.
   
      \note   If the state is initialized to 0, the LFSR will always generate
              '0' bits.
   */
  UINT32 state;
  
  /*! \var    initial_state
      \brief  Tracks the initial state of the spreading code so that it can be
              reset at a later point in time.
   */
  UINT32 initial_state;
  
} spreading_code;

/*! \fn     csignal_error_code csignal_initialize_spreading_code (
             UCHAR            in_degree,
             UINT32           in_generator_polynomial,
             UINT32           in_initial_state,
             spreading_code** out_spreading_code
            )
    \brief  Initializes the sequence generator struct, spreading_code. It
            also ensures that the generator polynomial is valid.
 
    \note   The LSB of the polynomial must be a '1'. Since we are dealing with
            simple LFSP the g_r term is always assumed to be 1.
 
    \param  in_degree The degree of the generator polynomial.
    \param  in_generator_polynomial The generator polynomial for the LFSR.
    \param  in_initial_state  The initial state of the LFSR. If this value is 0
                              the generator will only ever generate 0s.
    \param  out_spreading_code  If no errors are detected a new spreading_code
                                struct will be created and returned. The caller
                                must free this structure.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_spreading_code is NULL
            CPC_ERROR_CODE_INVALID_PARAMETER  If the LSB in the polynomial is
                                              not '1', if degree is not in the
                                              range [2,32], or if bits are set
                                              outside of the bit positions for
                                              g_r, g_{r-1}, ..., g_0.
 */
csignal_error_code
csignal_initialize_spreading_code (
                                   UCHAR            in_degree,
                                   UINT32           in_generator_polynomial,
                                   UINT32           in_initial_state,
                                   spreading_code** out_spreading_code
                                   );

/*! \fn     csignal_error_code csignal_get_spreading_code  (
             spreading_code*  in_spreading_code,
             USIZE            in_number_of_code_bits,
             USIZE*           out_size,
             UCHAR**          out_code
            )
    \brief  Gets the next in_number_of_bits from the sequence generator
            configured in in_spreading_code. The generated bits are returned
            as a byte array in out_code. The size of the byte array is in
            out_size.

    \note   Bits are packed into out_spreading_code starting with the MSB
            of each byte, i.e., if one bit is requested it will be stored in
            the MSB of out_spreading_code[ 0 ].
 
    \param  in_spreading_code The generator and initial state of the LFSR.
    \param  in_number_of_code_bits The number of code bits to get from the LFSR.
    \param  out_size  The size of the byte array containing the code bits (in
                      bytes).
    \param  out_code  The code bits generated
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc and csignal_get_spreading_code_bit for other
            possible errors):

            CPC_ERROR_CODE_NULL_POINTER If in_spreading_code, out_code, or 
                                        out_size are Null.
            CPC_ERROR_CODE_INVALID_PARAMETER  If in_number_of_bits is zero
 */
csignal_error_code
csignal_get_spreading_code  (
                             spreading_code*  in_spreading_code,
                             USIZE            in_number_of_code_bits,
                             USIZE*           out_size,
                             UCHAR**          out_code
                             );

/*! \fn     csignal_error_code csignal_set_spreading_signal  (
             UCHAR    in_bit,
             USIZE    in_signal_size,
             FLOAT64* out_signal
            )
    \brief  Generates a spreading signal based on in_bit. If in_bit is non-zero
            than a signal consisting of +1 values is generated, otherwise
            a signal consisting of -1 values is generated.
 
    \param  in_bit  Bit value which determines if the signal will consist of +1
            or -1 values, which will occurr for non-zero and zero bit values
            respectively.
    \param  in_signal_size  The length of the signal buffer
    \param  out_signal  An empty buffer to be filled with samples of +1 or -1.
    \return Returns NO_ERROR upon succesful exection or one of these errors:
      
            CPC_ERROR_CODE_NULL_POINTER If out_signal is null.
 */
csignal_error_code
csignal_set_spreading_signal  (
                               UCHAR    in_bit,
                               USIZE    in_signal_size,
                               FLOAT64* out_signal
                               );

/*! \fn     csignal_error_code csignal_destroy_spreading_code (
              spreading_code* io_spreading_code
            )
    \brief  Function to handle releasing the spreading code struct.

    \param  io_spreading_code  The spreading code to free.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_free for other possible errors):

            CPC_ERROR_CODE_NULL_POINTER If io_spreading_code is NULL
*/
csignal_error_code
csignal_destroy_spreading_code(
                                spreading_code* io_spreading_code
                              );

/*! \fn     csignal_error_code csignal_reset_spreading_code (
             spreading_code* io_spreading_code
            )
    \brief  Resets the code sequence generator to its intiai values (i.e.,
            its initial state).
 
    \param  io_spreading_code  The code generator to reset.
    \return Returns NO_ERROR upon succesful execution or one of these errors:
        
            CPC_ERROR_CODE_NULL_POINTER If io_spreading_code is NULL.
 */
csignal_error_code
csignal_reset_spreading_code  (
                               spreading_code* io_spreading_code
                               );

#endif  /*  __SPREADING_CODE_H__  */
