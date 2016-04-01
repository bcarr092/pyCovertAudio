/*! \file   ios_cahal.h
    \brief  iOS platform specific calls to initialize the audio API.
 
    \author Brent Carrara
 */
#ifndef __IOS_CAHAL_H__
#define __IOS_CAHAL_H__

#include <CoreFoundation/CoreFoundation.h>
#include <AudioToolbox/AudioToolbox.h>

#include <darwin_helper.h>

#include "cahal.h"

#include "ios_cahal_device.h"

/*! \fn     void ios_interrupt_listener  (
              void* in_user_data,
              UINT32 in_state
            )
    \brief  Callback called when the application's use of the audio hardware
            is interrupted. This handle currently just exits as the library
            doesn't support being interrupted.
 
    \param  in_user_data  Currently not used.
    \param  in_state  The state indicating if the interruption is beginning or
                      ending.
 */
void
ios_interrupt_listener  (
                         void* in_user_data,
                         UINT32 in_state
                         );

/*! \fn     OSStatus ios_initialize_audio_session( void )
    \brief  Initialization code for the AudioSession library. This function will
            setup the interrupt handler and indicate to the OS that this library
            will access the audio hardware in mode/category PlayAndRecord/
            VideoRecording. The AudioSession will be set to active as well.
 */
OSStatus
ios_initialize_audio_session( void );

#endif  /*  __IOS_CAHAL_H__ */
