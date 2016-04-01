/*! \file   cpcommon.h
    \brief  This is the cross-platform entry point to the cpcommon lib. This is
            the only file that external libraries/binaries should include. All
            platform specfic functionality should be defined in this library
            and implemented in a platform-specific way while presenting a
            common external interface.
 
    \author Brent Carrara
 */
#ifndef __CPCOMMON_H_
#define __CPCOMMON_H_

#include <stdio.h>
#include <stdarg.h>

#include "types.h"
#include "log_definitions.h"
#include "log_functions.h"
#include "cpcommon_error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*! \def    CPC_CODE_STRING_LENGTH
      \brief  The length of the string representation of a cpc code.
      */
#define CPC_CODE_STRING_LENGTH 7

  /*! \def    CPC_PRINT_CODE
      \brief  Wrapper for cpc_print_code to ensure the correct file and line are
      printed in the log message when displaying the string for the error
      code.
      */
#define CPC_PRINT_CODE( in_log_level, in_code ) \
  cpc_print_code( in_log_level, __FILE__, __LINE__, in_code )

  /*! \def    CPC_MIN
      \brief  Generic macro to ensure the correct min function is called for the
      appropriate data type.
      */
#define CPC_MIN( type, in_a, in_b ) cpc_min_ ## type( in_a, in_b )

  /*! \def    CPC_POW
      \brief  Generic macro to ensure the correct power function is called for the
      appropriate data type.
      */
#define CPC_POW( type, in_base, in_power ) CPC_POW_ ## type( in_base, in_power )

  /*! \def    CPC_LOGARITHM_10
      \brief  Generic macro to ensure the correct log10 function is called for the
      appropriate data type.
      */
#define CPC_LOGARITHM_10( type, in_number ) CPC_LOG_10_ ## type( in_number )

  /*! \def    CPC_CEIL
      \brief  Generic macro to ensure the correct ceil function is called for the
      appropriate data type.
      */
#define CPC_CEIL( type, in_number ) CPC_CEIL_ ## type( in_number )

  /*! \def    CPC_FLOOR
      \brief  Generic macro to ensure the correct ceil function is called for the
      appropriate data type.
      */
#define CPC_FLOOR( type, in_number ) CPC_CEIL_ ## type( in_number )

  /*! \def    CPC_SQRT
      \brief  Generic macro to ensure the correct sqrt function is called for the
      appropriate data type.
      */
#define CPC_SQRT( type, in_number ) CPC_SQRT_ ## type( in_number )

  /*! \def    CPC_EXP
      \brief  Generic macro to ensure the correct exp function is called for the
      appropriate data type.
      */
#define CPC_EXP( type, in_number ) CPC_EXP_ ## type( in_number )

  /*! \def    CPC_ROUND
      \brief  Generic macro to ensure the correct round function is called for the
      appropriate data type.
      */
#define CPC_ROUND( type, in_number ) CPC_ROUND_ ## type( in_number )

  /*! \def    CPC_CEIL_FLOAT64
      \brief  Macro that maps to the platform specific double ceiling function.
      */
#define CPC_CEIL_FLOAT64( in_number ) ceil( in_number )

  /*! \def    CPC_FLOOR_FLOAT64
      \brief  Macro that maps to the platform specific double ceiling function.
      */
#define CPC_FLOOR_FLOAT64( in_number ) floor( in_number )

  /*! \def    CPC_LOG_10_FLOAT64
      \brief  Macro that maps to the platfrom specific double logarithm (base 10)
      function.
      */
#define CPC_LOG_10_FLOAT64( in_number ) log10( in_number )

  /*! \def    CPC_POW_FLOAT64
      \brief  Macro that maps to the platform specific double power function.
      */
#define CPC_POW_FLOAT64( in_base, in_exponent ) pow( in_base, in_exponent )

  /*! \enum  cpcommon_states
  *  \brief  The states that the library goes through. The state transition
  *          diagram is as follows NOT_INITIALIZED -> INITIALIZED -> TERMINATED.
  *          Currently, going back to NOT_INITIALIZED after TERMINATED is not
  *          supported.
  */
  enum cpcommon_states
  {
    CPCOMMON_STATE_NOT_INITIALIZED = 0,
    CPCOMMON_STATE_INITIALIZED,
    CPCOMMON_STATE_TERMINATED
  };

  /*! \var    cpcommon_state
  *  \brief  The global CPCommon library state variable
  */
  typedef UINT32 cpcommon_state;

  /*! \var    g_cpcommon_state
  *  \brief  This is the global state of the CPCommon library. This value should
  *          never be modified directly. It is updated automatically through
  *          specific API calls. To initialize the library call cpc_initialize,
  *          when done with the library call cpc_terminate.
  */
  extern cpcommon_state g_cpcommon_state;

  /*! \fn     void cpc_initialize( void )
      \brief  Initializes the library.
   */
  void
  cpc_initialize( void );

  /*! \fn     void cpc_terminate( void )
      \brief  Terminates the library.
  */
  void
  cpc_terminate( void );

  /*! \fn     CPC_BOOL cpc_is_initialized( void )
      \brief  Returns CPC_TRUE if the library is initialized CPC_FALSE otherwise.
   */
  CPC_BOOL
  cpc_is_initialized( void );

  /*! \fn     void platform_initialize( void )
      \brief  Performs platform specific initialization.
   */
  void
  platform_initialize( void );

  /*! \fn     void platform_terminate( void )
      \brief  Performs platform specific termination of the library.
   */
  void
  platform_terminate( void );

  /*! \fn     UINT32 cpc_min_UINT32  (
                UINT32 in_a,
                UINT32 in_b
                )
                \brief  Min function for the UINT32 data type.

                \param  in_a  The number to compare to in_b
                \param  in_b  The number to compare to in_a
                \return The minimum of in_a and in_b, in_b if they are equal.
                */
  UINT32
  cpc_min_UINT32(
                   UINT32 in_a,
                   UINT32 in_b
                   );

  /*! \fn     USIZE cpc_min_USIZE  (
                USIZE in_a,
                USIZE in_b
                )
                \brief  Min function for the UINT32 data type.

                \param  in_a  The number to compare to in_b
                \param  in_b  The number to compare to in_a
                \return The minimum of in_a and in_b, in_b if they are equal.
                */
  USIZE
  cpc_min_USIZE(
                   USIZE in_a,
                   USIZE in_b
                   );

  /*! \fn     FLOAT32 cpc_min_FLOAT32  (
                FLOAT32 in_a,
                FLOAT32 in_b
                )
                \brief  Min function for the FLOAT32 data type.

                \param  in_a  The number to compare to in_b
                \param  in_b  The number to compare to in_a
                \return The minimum of in_a and in_b, in_b if they are equal.
                */
  FLOAT32
  cpc_min_FLOAT32(
                   FLOAT32 in_a,
                   FLOAT32 in_b
                   );

  /*! \fn     FLOAT64 cpc_min_FLOAT64  (
                FLOAT64 in_a,
                FLOAT64 in_b
                )
                \brief  Min function for the FLOAT64 data type.

                \param  in_a  The number to compare to in_b
                \param  in_b  The number to compare to in_a
                \return The minimum of in_a and in_b, in_b if they are equal.
                */
  FLOAT64
  cpc_min_FLOAT64(
                   FLOAT64 in_a,
                   FLOAT64 in_b
                   );

  /*! \fn     INT32 cpc_snprintf  (
                CHAR**        io_string,
                USIZE         in_string_length,
                const CHAR*   in_string_format,
                ...
                )
                \brief  Cross-platform wrapper for vsnprintf

                \param  io_string Pointer to a buffer that will be created, populated, and
                returned.
                \param  in_string_length  The length of the buffer, io_string
                \param  in_string_format  The printf formatted format string
                \param  ...       The variable length argument list to use in formatting
                the string.
                \return Returns either a positive number indicating the number of characters
                that would have been written to io_string if io_string were of
                unlimited length. An error is returned otherwise. See the platform
                specific description for vsnprintf (or equivalent).m
                */
  INT32
  cpc_snprintf(
                 CHAR**        io_string,
                 USIZE         in_string_length,
                 const CHAR*   in_string_format,
                 ...
                 );

  /*! \fn     INT32 cpc_printf  (
                const CHAR* in_string_format,
                ...
                )
                \brief  Cross-platform wrapper for vprintf

                \param  in_string_format  The printf formatted format string
                \param  ... The variable length argument list to use in formatting the
                output print string.
                \return Returns either the number of characters output, or a negative
                number indicating an error. See the platform specific descrtiption
                of errors for vprintf (or equivalent).
                */
  INT32
  cpc_printf(
               const CHAR* in_string_format,
               ...
  );

  /*! \fn     INT32 cpc_vprintf  (
              const CHAR* in_string_format,
              va_list     in_list
              )
      \brief  Cross-platform wrapper for vprintf

      \param  in_string_format  The printf formatted format string.
      \param  in_list The variable length argument list to use in formatting the
      output string.
      \return Returns either the number of characters output, or a negative
      number indicating an error. See the platform specific descrtiption
      of errors for vprintf (or equivalent).
   */
  INT32
  cpc_vprintf(
                const CHAR* in_string_format,
                va_list     in_list
                );

  /*! \fn     INT32 cpc_fprintf (
                FILE* in_handle,
                const CHAR* in_string_format,
                ...
                )
                \brief  Cross-platform wrapper for vfprintf

                \param  in_handle The file handle to write the log to.
                \param  in_string_format  The printf formatted format string.
                \param  ... The variable length argument list to use in formatting the
                output print string.
                \return Returns either the number of characters output, or a negative
                number indicating an error. See the platform specific descrtiption
                of errors for vfprintf (or equivalent).
                */
  INT32
  cpc_fprintf(
               FILE* in_handle,
               const CHAR* in_string_format,
               ...
               );

  /*! \fn     INT32 cpc_vfprintf (
              FILE*       in_handle,
              const CHAR* in_string_format,
              va_list     in_list
              )
      \brief  Cross-platform wrapper for vfprintf

      \param  in_handle The file handle to write the log to.
      \param  in_string_format  The printf formatted format string.
      \param  in_list   The variable length argument list to use in formatting
      the output print string.
      \return Returns either the number of characters output, or a negative
      number indicating an error. See the platform specific descrtiption
      of errors for vfprintf (or equivalent).
   */
  INT32
  cpc_vfprintf(
                FILE*       in_handle,
                const CHAR* in_string_format,
                va_list     in_list
                );

  /*! \fn     void cpc_print_code (
                cpc_log_level  in_log_level,
                CHAR*          in_file,
                INT32          in_line_number,
                UINT32       in_code
                )
                \brief  Converts in_code to its string representation if possible and
                outputs it using the logger. This function tries to convert return
                codes (UINT32) that have four character representations, i.e. each
                byte is printable.

                \param  in_log_level  The log level to log the converted string
                \param  in_file       The file name to display in the log
                \param  in_line_number  The line number to be displayed in the log
                \param  in_code       The code to convert to its four character string
                representation.
                */
  void
  cpc_print_code(
                  cpc_log_level  in_log_level,
                  CHAR*          in_file,
                  INT32          in_line_number,
                  UINT32         in_code
                  );

  /*! \fn     CHAR* cpc_convert_code_to_cstring  (
                UINT32 in_code
                )
                \brief  Converts in_code to its string representation if possible and
                returns the newly created string. This function tries to convert
                UINT32 codes that have four character representations. This function
                will create a new CHAR*, populate it with the string representation
                of the code and return the string. It is up to the caller to free
                the string. A string representation is only generated when possible,
                i.e. when each character is printable.

                \param  in_code The code to convert to a c-style string, iff all bytes of
                in_code are printable ASCII characters.
                \return A newly created string representation of in_code if possible. The
                caller must free the returned string. A NULL string is returned if
                all the bytes of in_code are not printable ASCII.
                */
  CHAR*
  cpc_convert_code_to_cstring(
                                UINT32 in_code
                                );

  /*! \fn     UINT32 cpc_errno( void )
      \brief  Simply returns the system specific error number field.

      \return The platform specific errno.
      */
  UINT32
  cpc_errno( void );

  /*! \fn     CHAR* cpc_strerror  (
                UINT32 in_error
                )
                \brief  Wraps the platform-specific strerror function. Used to translate
                errors in system calls to a string.

                \param  in_error  The error to stringify.
                \return The string representation of in_error.
                */
  CHAR*
  cpc_strerror(
                 UINT32 in_error
                 );

  /*! \fn     cpc_error_code cpc_safe_malloc (
                void** out_pointer,
                SSIZE  in_buffer_size
                )
      \brief  Creates a safe implementation of malloc. This function will safely
              allocate a new memory buffer, zero it out and return it. If either
              the malloc or memset fail this function will return an error.
              CPC_ERROR_CODE_NO_ERROR is returned otherwise.

      \param  out_pointer Upon return out_pointer will point to a newly
                          allocated buffer or NULL if an error occurred.
      \param  in_buffer_size  The size of the buffer to create.
      \return CPC_ERROR_CODE_NO_ERROR(0) if no error has occurred, an error code
      otherwise.
*/
  cpc_error_code
  cpc_safe_malloc(
                   void** out_pointer,
                   SSIZE  in_buffer_size
                   );

  /*! \fn     cpc_error_code cpc_safe_free (
                void** io_pointer
                )
                \brief  This function is a safe implementation of free. This function
                will free io_pointer if it is a valid pointer and set it to NULL.
                If an error occurs the appropriate error code will be returned.

                \param  io_pointer  A pointer to the buffer to be freed.
                \return CPC_ERROR_CODE_NO_ERROR(0) if no error has occurred, an error code
                otherwise.
                */
  cpc_error_code
  cpc_safe_free(
                 void** io_pointer
                 );

  /*! \fn     void cpc_exit  (
               UINT32 in_exit_code
               )
               \brief  Function simply logs the exit code then exits the application.

               \note   This function does not return.

               \param  in_exit_code  The exit code to log before terminating.
               */
  void
  cpc_exit(
             UINT32 in_exit_code
             );

  /*! \fn     cpc_error_code cpc_strnlen (
                CHAR*   in_string,
                SSIZE*  io_length
                )
                \brief  Returns the length of the string up to a maximum.

                \param  in_string The string whose length is to be computed.
                \param  io_length On input the max length of in_string is passed in. On
                return io_length contains the lenght of in_string up to
                the max passed in. The length doees not count the null-
                terminator.
                \return NO_ERROR if there is no error, an appropriate error code otherwise.
                */
  cpc_error_code
  cpc_strnlen(
      CHAR*   in_string,
      SSIZE*  io_length
              );

  /*! \fn     cpc_error_code cpc_memcpy  (
                void* out_destination,
                void* in_source,
                SSIZE in_num_bytes
                )
                \brief  Copies in_num_bytes from in_source to out_distination.

                \param  out_destination The location to copy in_source to. Must not be null.
                \param  in_source The location to copy from.
                \param  in_num_bytes  The number of bytes to copy from in_source to
                out_destination.
                \return NO_ERROR if there is no error, an appropriate error code otherwise.
                */
  cpc_error_code
  cpc_memcpy(
      void* out_destination,
      void* in_source,
      SSIZE in_num_bytes
              );

  /*! \fn     cpc_error_code cpc_safe_realloc  (
                void**  io_pointer,
                SSIZE   in_original_size,
                SSIZE   in_new_size
                )
                \brief  Reallocates io_pointer to a buffer of size in_new_size and copies
                the contents of io_pointer to the new buffer. Frees the old buffer.

                \param  io_pointer  Pointer to the buffer that is to be reallocated.
                \param  in_original_size  The length of the buffer pointed to by io_pointer.
                \param  in_new_size The desired size for io_pointer.
                \return NO_ERROR if there is no error, an appropriate error code otherwise.
                */
  cpc_error_code
  cpc_safe_realloc(
      void**  io_pointer,
      SSIZE   in_original_size,
      SSIZE   in_new_size
                    );

  /*! \fn     CPC_BOOL cpc_check_if_file_exists  (
                CHAR* in_file_name
                )
                \brief  Checks if in_file_name can be opened for reading and returns true
                if it can.

                \note This is not a comprehensive file check test. If the file exists but
                the process checking doesn't have read permissions this function will
                think the file does not exist.

                \param  in_file_name  The file path to check for existence.
                \return CPC_TRUE if the file can be opened for reading, CPC_FALSE otherwise.
                */
  CPC_BOOL
  cpc_check_if_file_exists(
                             CHAR* in_file_name
                             );

  /*! \fn     FLOAT64 cpc_bessel_i0 (
                FLOAT64 in_number
                )
                \brief  Calculates the modified Bessel function of the 0th order I_0(x). The
                implementation of this function was taken from:

                Numerical recipes in C: the art of scientific computing by Press, et
                al. (Chapter 6)

                \param  in_number The input to the Bessel funciton.
                \return The result of the modified Bessel function.
                */
  FLOAT64
  cpc_bessel_i0(
                 FLOAT64 in_number
                 );

#ifdef __cplusplus
}
#endif

#endif /* __CPCOMMON_H_ */

