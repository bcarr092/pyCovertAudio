/*! \file   log_functions.h
    \brief  Logging functions that allow for messages to be logged at multiple
            different logging levels are defined in this file.
 
    \author Brent Carrara
 */
#ifndef __LOG_H__
#define __LOG_H__

#include "cpcommon.h"
#include "cpcommon_error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /*! \def    CPC_LOG_STRING
      \brief  Logging function for strings. Used to log static messages.
      */
#define CPC_LOG_STRING( in_log_level, in_string ) \
  cpc_log( in_log_level, __FILE__, __LINE__, in_string )

  /*! \def    CPC_ERROR
      \brief  Shorthand macro to log an error. Simply calls CPC_LOG with the error
      log level.
      */
#define CPC_ERROR( in_log_format, ... ) \
  cpc_log( CPC_LOG_LEVEL_ERROR, __FILE__, __LINE__, in_log_format, __VA_ARGS__ )

  /*! \def    CPC_LOG
      \brief  Logging for function that provides a printf-like interface. This is
      the logging function that should be called as it will preserve the
      file and line number the log function call is made from.
      */
#define CPC_LOG( in_log_level, in_log_format, ... ) \
  cpc_log( in_log_level, __FILE__, __LINE__, in_log_format, __VA_ARGS__ )

  /*! \def    CPC_LOG_BUFFER
      \brief  Logging for function that prints char buffers in a hexdump format for
              analysis. This macro should be used instead of calling the
              cpc_log_buffer function directly as this function will preserve the
              file and line number.
  */
#define CPC_LOG_BUFFER( in_log_level, in_label, in_buffer, in_buffer_length, in_num_columns ) \
  cpc_log_buffer( in_log_level, __FILE__, __LINE__, in_label, in_buffer, in_buffer_length, in_num_columns )

  /*! \def    CPC_LOG_BUFFER_FLOAT32
      \brief  Logging for function that prints float buffers in a hexdump format for
              analysis. This macro should be used instead of calling the
              cpc_log_buffer function directly as this function will preserve the
              file and line number.
   */
#define CPC_LOG_BUFFER_FLOAT32( in_log_level, in_label, in_buffer, in_buffer_length, in_num_columns ) \
  cpc_log_buffer_FLOAT32( in_log_level, __FILE__, __LINE__, in_label, in_buffer, in_buffer_length, in_num_columns )

  /*! \def    CPC_LOG_BUFFER_FLOAT64
      \brief  Logging for function that prints float buffers in a hexdump format for
              analysis. This macro should be used instead of calling the
              cpc_log_buffer function directly as this function will preserve the
              file and line number.
   */
#define CPC_LOG_BUFFER_FLOAT64( in_log_level, in_label, in_buffer, in_buffer_length, in_num_columns ) \
  cpc_log_buffer_FLOAT64( in_log_level, __FILE__, __LINE__, in_label, in_buffer, in_buffer_length, in_num_columns )

  /*! \fn     cpc_error_code cpc_log  (
                cpc_log_level in_log_level,
                CHAR*         in_file,
                INT32         in_line_number,
                CHAR*         in_log_format,
                ...
                )
                \brief  Logging function that will either not output the log (if
                in_log_level is below the current log level) or output the log
                to either stdout or stderr (if in_log_level is the ERROR level).

                \param  in_log_level    The log level the message should be logged at
                \param  in_file         The file name cpc_log was called from
                \param  in_line_number  The line number cpc_log was called on
                \param  in_log_format   The printf formatted format string
                \param  ...             The variable length argument list to use in
                formatting the log.
                \return Either CPC_ERROR_CODE_NO_ERROR if no error is detected or an error.
                */
  cpc_error_code
  cpc_log(
            cpc_log_level in_log_level,
            CHAR*         in_file,
            INT32         in_line_number,
            CHAR*         in_log_format,
            ...
            );

  /*! \fn     cpc_error_code cpc_log_set_log_level (
                cpc_log_level in_new_log_level
                )
                \brief  Set the current log level to in_new_log_level

                \param  in_new_log_level  The log level to set the current log level to
                \return Either CPC_ERROR_CODE_NO_ERROR if the current log level is changed
                or an error code.
                */
  cpc_error_code
  cpc_log_set_log_level(
                         cpc_log_level in_new_log_level
                         );

  /*! \fn     cpc_log_get_current_log_level
      \brief  Provides access to the currently configured log level

      \return Returns the currently configured log level
      */
  cpc_log_level
  cpc_log_get_current_log_level( void );

  /*! \fn     CHAR* cpc_log_level_to_string ( cpc_log_level in_log_level )
      \brief  Translates the log level to its string representation

      \param  in_log_level  The log level to translate to a string.
      \return String representation of the given log level
      */
  CHAR*
  cpc_log_level_to_string(
                           cpc_log_level in_log_level
                           );

  /*! \fn     cpc_error_code cpc_log_buffer  (
                cpc_log_level  in_log_level,
                CHAR*          in_file,
                INT32          in_line_number,
                CHAR*          in_label,
                UCHAR*         in_buffer,
                SSIZE          in_buffer_length,
                UINT32         in_num_columns
                )
                \brief  Prints in_buffer in hexdump format for analysis.

                \param  in_log_level    The log level the message should be logged at
                \param  in_file         The file name cpc_log was called from
                \param  in_line_number  The line number cpc_log was called on
                \param  in_label        The label to print at the beginning of the log
                entry.
                \param  in_buffer The buffer to print in hexdump format.
                \param  in_buffer_length  The size of in_buffer in bytes.
                \param  in_num_columns    The number of columns to display. One byte is
                printed per column.
                \return Either CPC_ERROR_CODE_NO_ERROR if no error is detected or an error.
                */
  cpc_error_code
  cpc_log_buffer(
                   cpc_log_level  in_log_level,
                   CHAR*          in_file,
                   INT32          in_line_number,
                   CHAR*          in_label,
                   UCHAR*         in_buffer,
                   SSIZE          in_buffer_length,
                   UINT32         in_num_columns
                   );

  /*! \fn     cpc_error_code cpc_log_buffer_FLOAT32  (
                cpc_log_level  in_log_level,
                CHAR*          in_file,
                INT32          in_line_number,
                CHAR*          in_label,
                FLOAT32*       in_buffer,
                UINT32         in_buffer_length,
                UINT32         in_num_columns
                )
                \brief  Prints in_buffer in hexdump format for analysis.

                \param  in_log_level    The log level the message should be logged at
                \param  in_file         The file name cpc_log was called from
                \param  in_line_number  The line number cpc_log was called on
                \param  in_label        The label to print at the beginning of the log
                entry.
                \param  in_buffer The buffer to print in hexdump format.
                \param  in_buffer_length  The number of entries in in_buffer.
                \param  in_num_columns    The number of columns to display. One byte is
                printed per column.
                \return Either CPC_ERROR_CODE_NO_ERROR if no error is detected or an error.
                */
  cpc_error_code
  cpc_log_buffer_FLOAT32(
                           cpc_log_level  in_log_level,
                           CHAR*          in_file,
                           INT32          in_line_number,
                           CHAR*          in_label,
                           FLOAT32*       in_buffer,
                           UINT32         in_buffer_length,
                           UINT32         in_num_columns
                           );

  /*! \fn     cpc_error_code cpc_log_buffer_FLOAT64  (
                cpc_log_level  in_log_level,
                CHAR*          in_file,
                INT32          in_line_number,
                CHAR*          in_label,
                FLOAT64*       in_buffer,
                UINT32         in_buffer_length,
                UINT32         in_num_columns
                )
                \brief  Prints in_buffer in hexdump format for analysis.

                \param  in_log_level    The log level the message should be logged at
                \param  in_file         The file name cpc_log was called from
                \param  in_line_number  The line number cpc_log was called on
                \param  in_label        The label to print at the beginning of the log
                entry.
                \param  in_buffer The buffer to print in hexdump format.
                \param  in_buffer_length  The number of entries in in_buffer.
                \param  in_num_columns    The number of columns to display. One byte is
                printed per column.
                \return Either CPC_ERROR_CODE_NO_ERROR if no error is detected or an error.
                */
  cpc_error_code
  cpc_log_buffer_FLOAT64(
                           cpc_log_level  in_log_level,
                           CHAR*          in_file,
                           INT32          in_line_number,
                           CHAR*          in_label,
                           FLOAT64*       in_buffer,
                           UINT32         in_buffer_length,
                           UINT32         in_num_columns
                           );

#ifdef __cplusplus
}
#endif

#endif /* __LOG_H__ */
