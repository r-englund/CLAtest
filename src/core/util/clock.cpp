#include <inviwo/core/util/clock.h>

static const double CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000.0;
static const double INV_CLOCKS_PER_MS = 1.0 / CLOCKS_PER_MS;


namespace inviwo{

    void Clock::start(){
        startTime_ = clock();
    }


    void Clock::stop(){
        stopTime_ = clock();
    }

    unsigned int Clock::getElapsedMiliseconds()const{
        return (stopTime_ - startTime_)*INV_CLOCKS_PER_MS;
    }


    unsigned int Clock::getElapsedSeconds()const{
        return (stopTime_ - startTime_)/CLOCKS_PER_SEC;
    }




}//namespace