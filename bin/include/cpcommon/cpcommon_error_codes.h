/*! \file   cpcommon_error_codes.h
    \brief  File containing the enum of errors used by the logger.
 
    \author Brent Carrara
 */
#ifndef __CPCOMMON_ERROR_CODES_H__
#define __CPCOMMON_ERROR_CODES_H__

#include "types.h"

/*! \enum   cpc_error_codes
    \brief  The error codes used by the logger
 
    \var CPC_ERROR_CODE_NO_ERROR
                Used to indicate no error.
    \var CPC_ERROR_CODE_NULL_POINTER
                Code used when an operation is beign attempted on a null pointer
    \var CPC_ERROR_CODE_INVALID_LOG_LEVEL
                Used by the logger to indicate an unsupported log level has been
                used in a log function.
    \var CPC_ERROR_CODE_MALLOC
                Used by cpc_safe_malloc to indicate that the system call to
                malloc has failed.
    \var CPC_ERROR_CODE_MEMSET
                Used by cpc_safe_malloc to indidate that the system call to
                memset has failed.
    \var CPC_ERROR_CODE_INVALID_PERMISSIONS
                Used when an operation cannot be performed because the correct
                permissions have not been obtained.
    \var CPC_ERROR_CODE_INVALID_PARAMETER
                Used when a parameter passed to a function is invalid.
    \var CPC_ERROR_CODE_API_ERROR
                Used when a native OS API is called and it returns an error.
 */
enum cpc_error_codes
{
  CPC_ERROR_CODE_NO_ERROR                       = 0,
  CPC_ERROR_CODE_NULL_POINTER                   = -1,
  CPC_ERROR_CODE_INVALID_LOG_LEVEL              = -2,
  CPC_ERROR_CODE_MALLOC                         = -3,
  CPC_ERROR_CODE_MEMSET                         = -4,
  CPC_ERROR_CODE_APPLICATION_INTERRUPTED_IOS    = -5,
  CPC_ERROR_CODE_INVALID_PERMISSIONS            = -6,
  CPC_ERROR_CODE_INVALID_PARAMETER              = -7,
  CPC_ERROR_CODE_API_ERROR                      = -8
};

/*! \var    cpc_error_code
    \brief  Type definition for the loggers errors
 */
typedef INT32 cpc_error_code;

#endif /* __CPCOMMON_ERROR_CODES_H__ */
