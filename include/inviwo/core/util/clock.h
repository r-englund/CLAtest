
#ifndef IVW_CLOCK_H
#define IVW_CLOCK_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

/** \class Clock 
*
* Class for measure time.
*/
class IVW_CORE_API Clock {
public:
    Clock(){}
    virtual ~Clock(){}

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
    unsigned int getElapsedMiliseconds()const;
   
    /**
    * Returns the amount of seconds, as float with millisecond precision in decimal places, between start and stop call.
    * If neither start or stop has been called, the return value is undefined.
    */
    float getElapsedSecondsWithMiliseconds()const{return getElapsedMiliseconds()/1000.0f;}

    
    /**
    * Returns the amount of seconds between start and stop call.
    * If neither start or stop has been called, the return value is undefined.
    */
    unsigned int getElapsedSeconds()const;
protected:

    clock_t startTime_;
    clock_t stopTime_;;

};

}; // namespace inviwo

#endif // IVW_TIMER_H