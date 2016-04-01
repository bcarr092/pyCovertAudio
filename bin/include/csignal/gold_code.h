/*! \file   gold_code.h
    \brief  Contains the headers for generating gold codes. Gold codes are used
            in CDMA applications and provide each user with their own unique
            spreading code that is nearly uncorrelated with all other codes.
 
    \author Brent Carrara
 */
#ifndef __GOLD_CODE_H__
#define __GOLD_CODE_H__

#include "spreading_code.h"
#include "csignal_error_codes.h"

/*! \var    gold_code
    \brief  A gold code sequence is generated through the use of two separate
            LFSRs, both of the same degree (r), each composed of m-sequences,
            but that follow specific conditions. Pairs of m-sequences that
            satisfy these conditions are called 'preferred pairs.' For a
            description of the conditions that must be satisfied for preferred
            pairs see 'Introduction to Spread Spectrum Communications' by
            Peterson, et al., Chapter 3.
 
            Each gold code can generate N + 2 sequences that when their
            cross-correlation is taken produce minimal values. Gold codes are
            ideal for generating spreading sequences to allow for multiple
            access to a shared frequency band (as in CDMA). To generate each of
            the codes use the following configurations:
    
            g_1, g_2, i_s_1, 0
            g_1, g_2, 0, i_s_2,
            g_1, g_2, i_s_1, i_s_2
            g_1, g_2, i_s_1, s_2
 
            where, g_1 and g_2, are the preferred pair generators, i_s_1 and
            i_s_2 are the initial states of the two LFSRs, s_2 is used to
            indicate the other N - 1 initial states that are non-zero and not 
            equal to i_s_2 or 0, and 0 is used to indicate the all-zero initial
            state.
 
    \note   N is the period of the sequence (i.e. bits repeat after N bits) and
            is caculated as N = 2^r - 1
 */
typedef struct gold_code_t
{
  /*! \var    polynomials
      \brief  The two separate LFSRs used to generate gold codes.
   */
  spreading_code* polynomials[ 2 ];
  
} gold_code;

/*! \fn     csignal_error_code csignal_initialize_gold_code  (
              UCHAR        in_degree,
              UINT32       in_generator_polynomial_1,
              UINT32       in_generator_polynomial_2,
              UINT32       in_initial_state_1,
              UINT32       in_initial_state_2,
              gold_code**  out_gold_code
            )
    \brief  Initializes a gold code generator struct, i.e. two LFSRs. It
            also ensures that the generator polynomials are valid.
 
    \note   The LSB of both polynomials must be a '1'. Since we are dealing with
            simple LFSP the g_r term is always assumed to be 1.
 
    \param  in_degree The degree of the generator polynomials.
    \param  in_generator_polynomial_1 The generator polynomial for the first
            LFSR.
    \param  in_generator_polynomial_2 The generator polynomial for the second
            LFSR.
    \param  in_initial_state_1  The initial state of the first LFSR.
    \param  in_initial_state_2  The initial state of the second LFSR.
    \param  out_gold_code If no errors are detected a new spreading_code
                          struct will be created and returned. The caller
                          must free this structure.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc and csignal_initialize_spreading_code for other
            possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_gold_code is NULL
 */
csignal_error_code
csignal_initialize_gold_code  (
                               UCHAR        in_degree,
                               UINT32       in_generator_polynomial_1,
                               UINT32       in_generator_polynomial_2,
                               UINT32       in_initial_state_1,
                               UINT32       in_initial_state_2,
                               gold_code**  out_gold_code
                               );

/*! \fn     csignal_error_code csignal_destroy_gold_code (
              gold_code* io_gold_code
            )
    \brief  Function to handle releasing the gold code struct.
 
    \param  io_gold_code  The gold code to free.
    \return Returns NO_ERROR upon succesful execution or one of these errors
           (see cpc_safe_free for other possible errors):
           
           CPC_ERROR_CODE_NULL_POINTER If out_gold_code is NULL
 */
csignal_error_code
csignal_destroy_gold_code (
                           gold_code* io_gold_code
                           );

/*! \fn     csignal_error_code csignal_get_gold_code  (
              gold_code*  in_gold_code,
              USIZE       in_number_of_code_bits,
              USIZE*      out_size,
              UCHAR**     out_code
            )
    \brief  Gets the next in_number_of_bits from the gold code sequence
            generator configured in in_gold_code. The generated bits are
            returned as a byte array in out_code. The size of the byte array is
            in out_size.
 
    \param  in_gold_code  The generator and initial state of the gold code
                          sequence generator.
    \param  in_number_of_code_bits The number of code bits to get from the LFSRs.
    \param  out_size  The size of the byte array containing the code bits (in
                      bytes).
    \param  out_code  The code bits generated
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc and csignal_get_spreading_code for other
            possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If in_gold_code, out_code, or
                                        out_size are Null.
            CPC_ERROR_CODE_INVALID_PARAMETER  If in_number_of_bits is zero
 */
csignal_error_code
csignal_get_gold_code  (
                        gold_code*  in_gold_code,
                        USIZE       in_number_of_code_bits,
                        USIZE*      out_size,
                        UCHAR**     out_code
                        );

/*! \fn     csignal_error_code csignal_reset_gold_code (
             gold_code* io_gold_code
            )
    \brief  Resets the gold code sequence generator to its intiai values (i.e.,
            its initial state).
 
    \param  io_gold_code  The Gold code generator to reset.
    \return Returns NO_ERROR upon succesful execution or one of these errors:
        
            CPC_ERROR_CODE_NULL_POINTER If io_gold_code or either of its
                                        generators is NULL.
 */
csignal_error_code
csignal_reset_gold_code (
                         gold_code* io_gold_code
                         );

#endif  /*  __GOLD_CODE_H__ */
