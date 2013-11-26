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

