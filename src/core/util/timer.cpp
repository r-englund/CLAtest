/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/util/timer.h>

namespace inviwo {

#ifdef WIN32

void CALLBACK TimerCallback( void* param, bool timerOrWaitFired )
{
    WindowsTimer* timer = static_cast<WindowsTimer*>(param);
    timer->onIntervalEvent();
}
WindowsTimer::WindowsTimer() :Timer(), timer_(NULL)
{

}

WindowsTimer::~WindowsTimer()
{
    stop();
}

void WindowsTimer::start( unsigned int intervalInMilliseconds, bool once /*= false*/ )
{
    if(timer_) {
        return;
    }
    BOOL success = CreateTimerQueueTimer( &timer_,
        NULL,
        (WAITORTIMERCALLBACK)TimerCallback,
        this,
        0,
        once ? 0 : intervalInMilliseconds,
        WT_EXECUTEINTIMERTHREAD);
}

void WindowsTimer::stop()
{
    DeleteTimerQueueTimer( NULL, timer_, NULL );
    timer_ = NULL ;
}

#endif // WIN32

} // namespace inviwo

