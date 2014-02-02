#include <inviwo/core/util/clock.h>

static const double CLOCKS_PER_MS = CLOCKS_PER_SEC / 1000.0;
static const double INV_CLOCKS_PER_MS = 1.0 / CLOCKS_PER_MS;


namespace inviwo {

Clock::Clock() {
#ifdef WIN32
    QueryPerformanceFrequency(&ticksPerSecond_);
#endif
}

void Clock::start() {
#ifdef WIN32
    QueryPerformanceCounter(&startTime_);
#else
    startTime_ = clock();
#endif
}

void Clock::stop() {
#ifdef WIN32
    QueryPerformanceCounter(&stopTime_);
#else
    stopTime_ = clock();
#endif
}

float Clock::getElapsedMiliseconds() const {
    return 1000.f*getElapsedSeconds();
}

float Clock::getElapsedSeconds() const {
#ifdef WIN32
    return static_cast<float>(stopTime_.QuadPart-startTime_.QuadPart) / ticksPerSecond_.QuadPart;
#else
    return static_cast<float>(stopTime_ - startTime_)/static_cast<float>(CLOCKS_PER_SEC);
#endif
}


}//namespace