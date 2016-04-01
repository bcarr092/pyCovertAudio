/*! \file   cahal.h
    \brief  This is the only header that an external library/binary should have 
            include. Provides a cross-platform abstraction layer to all the
            audio input/output hardware supported by the platform this library
            is run on.
 
    \author Brent Carrara
 */
#ifndef _CAHAL_H_
#define _CAHAL_H_

#include <cpcommon.h>

#include "cahal_device.h"
#include "cahal_device_stream.h"
#include "cahal_audio_format_flags.h"
#include "cahal_audio_format_description.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*! \def    MAJOR_VERSION
    \brief  The major version of the library.
 */
#define MAJOR_VERSION 1U
  
/*! \def    MINOR_VERSION
    \brief  The minor version of the library.
 */
#define MINOR_VERSION 0U

/*! \def    LIBRARY_NAME
    \brief  The name of the library.
 */
#define LIBRARY_NAME  "cahal"

/*! \enum   cahal_states
 *  \brief  The states that the library goes through. The state transition
 *          diagram is as follows NOT_INITIALIZED -> INITIALIZED -> TERMINATED.
 *          Currently, going back to NOT_INITIALIZED after TERMINATED is not
 *          supported.
 */
enum cahal_states
{
  CAHAL_STATE_NOT_INITIALIZED  = 0,
  CAHAL_STATE_INITIALIZED,
  CAHAL_STATE_TERMINATED
};

/*! \var    cahal_state
 *  \brief  The global CAHAL library state variable
 */
typedef UINT32 cahal_state;

/*! \var    g_cahal_state
 *  \brief  This is the global state of the CAHAL library. This value should
 *          never be modified directly. It is updated automatically through
 *          specific API calls. To initialize the library call cahal_initialize,
 *          when done with the library call cahal_terminate.
 */
extern cahal_state g_cahal_state;

/*! \fn     UINT16 cahal_get_version( void )
    \brief  Returns the version of the library in a SHORT. The top 8 MSBs are
            the major version and the 8 LSBs are the minor version.
 
    \return A short containing the major and minor version (8 MSBs are major,
            8 LSBs are minor)
 */
UINT16
cahal_get_version( void );

/*! \fn     CHAR* cahal_get_version_string( void )
    \brief  Returns a string containing the library name followed by the version
            of the library.
 
    \return A signed character array containing a string representation of the
            library with version. The caller must free the version string.
 */
CHAR*
cahal_get_version_string( void );

/*! \fn     void cahal_initialize( void )
    \brief  Entry point for the library. This must be the first function that
            external libraries call. If not, the system will not be 
            appropriately initialized and the behaviour of the library is
            undefined.
 */
void
cahal_initialize( void );

/*! \fn     void cahal_terminate( void )
 *  \brief  Termination point for the library. By calling this function all
 *          associated OS resources that are held by the library will be
 *          returned to the OS. The library is unusable after a call to
 *          terminate is performed.
 */
void
cahal_terminate( void );

/*! \fn     void cahal_sleep (
              UINT32 in_sleep_time
            )
    \brief  Platform-specific call to the sleep function. This method is only
            ever used for testing.

    \param  in_sleep_time The amount of time (in milliseconds) to put the thread
                          to sleep for.
 */
void
cahal_sleep (
    UINT32 in_sleep_time
            );

#ifdef __cplusplus
}
#endif

#endif /* _CAHAL_H_ */

