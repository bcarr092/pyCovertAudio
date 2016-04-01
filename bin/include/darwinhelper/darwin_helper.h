/*! \file   darwin_helper.h
    \brief  Darwin specific helper functions are defined in this file.
 
    \author Brent Carrara
 */
#ifndef __DARWIN_HELPER_H__
#define __DARWIN_HELPER_H__

#include <CoreFoundation/CoreFoundation.h>

#include <cpcommon.h>

/*! \fn     CHAR* darwin_convert_cfstring_to_char_string (
              CFStringRef in_string_to_convert
            )
    \brief  Helper function that creates a new c-style character string
            by converting in_string_to_convert to an ASCII formatted string.
            Note that the caller must free the return CHAR*.
 
    \param  in_string_to_convert  The CoreFoundation string to convert to a
                                  c-style string. The caller must free the
                                  returned string.
    \return A newly created c string that must be freed by the caller.
 */
CHAR*
darwin_convert_cfstring_to_char_string (
                                        CFStringRef in_string_to_convert
                                        );

#endif /* __DARWIN_HELPER_H__ */
