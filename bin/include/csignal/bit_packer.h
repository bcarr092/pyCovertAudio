/*! \file   bit_packer.h
    \brief  Definition for methods to store bits in a data structure.

    \author Brent Carrara
 */
#ifndef __BIT_PACKER_H__
#define __BIT_PACKER_H__

#include <cpcommon.h>

#include "csignal_error_codes.h"

/*! \def    BIT_PACKER_INITIAL_SIZE
    \brief  The initial size of the data buffer in the bit_packer struct.
 */
#define BIT_PACKER_INITIAL_SIZE     0x100

/*! \def    BIT_PACKER_GROWTH_RATE
    \brief  The rate at which to grow the data buffer in the bit_packer struct
            when more storage space is required.
 */
#define BIT_PACKER_GROWTH_RATE      0x2

/*! \var    bit_packer
    \brief  Struct definition for the bit packer. A bit packer is used to store
            data at the bit level.
 */
typedef struct bit_packer_t
{
  /*! \var    data
      \brief  The data buffer that contains the data bits
   */
  UCHAR* data;
  
  /*! \var    data_length
      \brief  The size of the data buffer
   */
  USIZE data_length;
  
  /*! \var    byte_offset
      \brief  A pointer to the buffer that data will be stored at.
   */
  USIZE byte_offset;
  
  /*! \var    bit_offset
      \brief  A pointer to the bit in byte_offset that data will be stored at.
   */
  UCHAR bit_offset;
  
} bit_packer;

/*! \fn     csignal_error_code bit_packer_initialize  (
              bit_packer**  out_bit_packer
            )
    \brief  Initializes the bit_packer struct with an empty data buffer and sets
            both bit and byte offsets to 0.
 
    \param  out_bit_packer  If this function successfully completes this
                            parameter will point to a newly created
                            bit_packer. It will be NULL otherwise.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If out_bit_packer is null
 */
csignal_error_code
bit_packer_initialize (
                       bit_packer** out_bit_packer
                       );

/*! \fn     csignal_error_code bit_packer_destroy  (
              bit_packer* in_bit_packer
            )
    \brief  Frees the struct in_bit_packer. Will also destroy the internal
            buffer data of the struct.
 
    \param  in_bit_packer The structure to free.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If in_bit_packer is null
 */
csignal_error_code
bit_packer_destroy  (
                     bit_packer* in_bit_packer
                     );

/*! \fn     csignal_error_code bit_packer_add_bytes (
              UCHAR*        in_data,
              USIZE         in_data_length,
              bit_packer*   io_bit_packer
            )
            )
    \brief  Adds the data in in_data to the bit packer.
 
    \param  in_data The data buffer that contains the bits to store.
    \param  in_data_length  The length of in_data
    \param  io_bit_packer  The bit packer struct to add the bits to.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_malloc and bit_packer_add_bits for other possible
            errors):
 
            CPC_ERROR_CODE_NULL_POINTER If in_data is null
 */
csignal_error_code
bit_packer_add_bytes (
                      UCHAR*        in_data,
                      USIZE         in_data_length,
                      bit_packer*   io_bit_packer
                      );

/*! \fn     csignal_error_code bit_packer_add_bits (
              UCHAR         in_data,
              USIZE         in_num_bits,
              bit_packer*   io_bit_packer
            )
            )
    \brief  Adds the in_num_bits LSBs in in_data to the bit packer.
 
    \note   This function will grow the data buffer that is stored within
            io_bit_packer.
 
    \param  in_data The data buffer that contains the bits to store.
    \param  in_num_bits  The number of LSBs in in_data to store.
    \param  io_bit_packer  The bit packer struct to add the bits to.
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_realloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If in_data is null
            CSIGNAL_ERROR_CODE_LENGTH_MISMATCH  If in_num_bits is larger than
                                                a UCHAR (in bits)
 */
csignal_error_code
bit_packer_add_bits (
                     UCHAR        in_data,
                     USIZE        in_num_bits,
                     bit_packer*  io_bit_packer
                     );

/*! \fn     csignal_error_code bit_packer_get_bytes (
              bit_packer* in_bit_packer,
              UCHAR** out_data,
              USIZE*   out_data_length
            )
    \brief  Copies the buffer from the packer to out_data (initializes out_data)
            and sets the length in out_data_length.
 
    \param  in_bit_packer The bit packer whose data is to be copied.
    \param  out_data  Pointer to a newly created buffer that stores the data
                      that was in in_bit_packer.
    \param  out_data_length The amount of data stored in out_data
    \return Returns NO_ERROR upon succesful execution or one of these errors
            (see cpc_safe_realloc for other possible errors):
 
            CPC_ERROR_CODE_NULL_POINTER If in_data is null
            CSIGNAL_ERROR_CODE_LENGTH_MISMATCH  If in_num_bits is larger than
                                                a UCHAR (in bits)
 */
csignal_error_code
bit_packer_get_bytes (
                      bit_packer* in_bit_packer,
                      UCHAR**     out_data,
                      USIZE*      out_data_length
                      );

#endif  /*  __BIT_PACKER_H__  */
