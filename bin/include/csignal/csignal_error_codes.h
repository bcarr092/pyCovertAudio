/*! \file   csignal_error_codes.h
    \brief  The error codes produced by the csignal library are defined here.
            These error codes are superset of error codes and extend the codes
            already defined in error_codes.h in the cpcommon library.
 
    \author Brent Carrara
 */
#ifndef __CSIGNAL_ERROR_CODES_H__
#define __CSIGNAL_ERROR_CODES_H__

/*! \enum   csignal_error_codes
    \brief  The error codes used by the csignal library
 
 \var CSIGNAL_ERROR_CODE_NO_DATA
      Used to indicate that no data is left in a buffer
 \var CSIGNAL_ERROR_CODE_WRITE_ERROR
      Used to indicate an error occurred when writing to a WAV file
 \var CSIGNAL_ERROR_CODE_LENGTH_MISMATCH
      Used to indidate that when generating a sequence of gold codes the
      component LFSRs did not generate the same number of bits.
 \var CSIGNAL_ERROR_CODE_INVALID_TYPE
      Used to indicate that the wrong type has been passed to the Python wrapper
      for the library.
 \var CSIGNAL_ERROR_CODE_NO_RESULT
      Used to indicate to the caller that the error that occurred is that there
      is no result.
 */
enum csignal_error_codes
{
  CSIGNAL_ERROR_CODE_NO_DATA                  = -100,
  CSIGNAL_ERROR_CODE_WRITE_ERROR              = -101,
  CSIGNAL_ERROR_CODE_LENGTH_MISMATCH          = -102,
  CSIGNAL_ERROR_CODE_INVALID_TYPE             = -103,
  CSIGNAL_ERROR_CODE_NO_RESULT                = -104,
};

/*! \var    csignal_error_code
    \brief  Type definition for the CSignal library errors
 */
typedef INT32 csignal_error_code;

#endif  /*  __CSIGNAL_ERROR_CODES_H__ */
