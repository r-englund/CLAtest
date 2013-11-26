#ifndef IVW_TIMER_H
#define IVW_TIMER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/callback.h>
#ifdef WIN32
// For WindowsTimer
#include <windows.h>
#endif

namespace inviwo {

/** \class Timer 
*
* Interface for Timer classes. 
* A class deriving from Timer should execute onIntervalEvent() when the interval has passed.
*/
class IVW_CORE_API Timer {
public:
    Timer() {};
    virtual ~Timer() { }

    /**
     * Start the timer.
     * 
     * @param intervalInMilliseconds The time interval until the added callback will be called.
     * @param once Should the callback only be called the first time the interval has been reached? 
     */
    virtual void start(unsigned int intervalInMilliseconds, bool once = false) = 0;

    /**
     * Stop the timer from calling the callback.
     * 
     * @return 
     */
    virtual void stop() {};

    /**
     * Set a callback that will be called when the time interval has passed.
     * Only one callback may be set.
     */
    template <typename T>
    void setElapsedTimeCallback(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }
    /**
     * This function will be called when the time has elapsed. 
     * Callbacks will then be executed.
     * 
     * @note Derived classes should call this function when the time has elapsed.
     */
    void onIntervalEvent() const {
        onChangeCallback_.invoke();
    }
protected:

    SingleCallBack onChangeCallback_;

};

#ifdef WIN32
/**
 * Will be called when WindowsTimer reaches time interval.
 * 
 * @param param Will be a pointer to WindowsTimer
 * @param timerOrWaitFired 
 */
static void CALLBACK TimerCallback(void* param, bool timerOrWaitFired);

/** \class WindowsTimer 
*
* Windows only timer. Uses the Windows API to create a timed event. Typically millisecond resolution.
* @note Does not work when used with Qt together with OpenGL (probably due to OpenGL context and parallel execution)
* @see Timer
*/
class IVW_CORE_API WindowsTimer: public Timer {
public:
    WindowsTimer();;
    virtual ~WindowsTimer();

    virtual void start(unsigned int intervalInMilliseconds, bool once = false);;
    virtual void stop();;


protected:
    HANDLE timer_;
};


#endif // WIN32

}; // namespace inviwo

#endif // IVW_TIMER_H