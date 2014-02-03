 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Rickard Englund, Daniel Jönsson
 *
 *********************************************************************************/

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