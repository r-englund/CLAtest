
#ifndef IVW_CLOCK_H
#define IVW_CLOCK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <time.h>

namespace inviwo {

/** \class Clock
*
* Class for measure time.
*/
class IVW_CORE_API Clock {
public:
    Clock();
    virtual ~Clock() {}

    /**
     * Start the Clock.
     *
     */
    void start();

    /**
     * Stops the clock.
     *
     */
    void stop();

    /**
    * Returns the amount of milliseconds between start and stop call.
    * If neither start or stop has been called, the return value is undefined.
    */
    float getElapsedMiliseconds() const;

    /**
    * Returns the amount of seconds between start and stop call.
    * If neither start or stop has been called, the return value is undefined.
    */
    float getElapsedSeconds() const;
protected:
#ifdef WIN32
    LARGE_INTEGER startTime_;
    LARGE_INTEGER stopTime_;
    LARGE_INTEGER ticksPerSecond_;
#else
    clock_t startTime_;
    clock_t stopTime_;
#endif




};

}; // namespace inviwo

#endif // IVW_TIMER_H