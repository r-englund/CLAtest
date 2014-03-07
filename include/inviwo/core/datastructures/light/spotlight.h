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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_SPOT_LIGHT_H
#define IVW_SPOT_LIGHT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/baselightsource.h>

namespace inviwo {

class SpotLight: public LightSource {
public:
    SpotLight(): LightSource() {}
    virtual ~SpotLight() {};

    virtual float getArea() const { return size_.x*size_.y; }
    /**
     * Get radiant flux (color) of light source.
     * @see setPower
     * @return Radiant flux in watt.
     */
    virtual vec3 getPower() const { return getIntensity()*getArea(); }


    LightSourceType::Enum getLightSourceType() const { return LightSourceType::LIGHT_CONE; }

    /**
     * Get world position of light source.
     *
     * @return World position of light source.
     */
    const vec3& getPosition() const { return position_; }

    /**
     * Set world position of light source.
     *
     * @param position World position of light source.
     */
    void setPosition(const vec3& position) { position_ = position; }

    /**
     * Get normalized general direction of light source.
     *
     * @return Normalized direction of light source.
     */
    const vec3& getDirection() const { return direction_; }

    /**
     * Set normalized direction of light source.
     *
     * @param direction Normalized direction of light source.
     */
    void setDirection(const vec3& direction) { direction_ = direction; }

    /**
     * Get cut off angle of the light source.
     *
     * @return Cut off angle of the light source.
     */
    const float& getCutOffAngle() const { return cutOffAngle_; }

    /**
     * Set the cut off angle of the light source.
     *
     * @param angle Cut off angle of the light source.
     */
    void setCutOffAngle(const float& angle) { cutOffAngle_ = angle; }

    /**
     * Get cut off factor of the light source.
     *
     * @return Cut off factor of the light source.
     */
    const float& getCutOffFactor() const { return cutOffFactor_; }

    /**
     * Set the cut off factor of the light source.
     *
     * @param angle Cut off factor of the light source.
     */
    void setCutOffFactor(const float& factor) { cutOffFactor_ = factor; }

    /**
     * Set the intensity (color) from the light source given in watt per steradian (flux density per solid angle, W*s*r^-1).
     *
     * @param power
     */
    void setIntensity(const vec3& intensity) { intensity_ = intensity; }

    /**
     * Get the intensity (color) from the light source given in watt per steradian (flux density per solid angle, W*s*r^-1).
     *
     * @return Flux density per solid angle, W*s*r^-1
     */
    const vec3& getIntensity() const { return intensity_; }

protected:
    vec3 position_;
    vec3 direction_;
    float cutOffAngle_;
    float cutOffFactor_;
    vec3 intensity_; // Color of light source, flux density per solid angle (given in watt per steradian W*s*r^-1)

};

} // namespace inviwo

#endif // IVW_SPOT_LIGHT_H