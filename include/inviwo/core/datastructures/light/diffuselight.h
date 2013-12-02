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

#ifndef IVW_DIFFUSE_LIGHT_H
#define IVW_DIFFUSE_LIGHT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/baselightsource.h>

namespace inviwo {

class DiffuseLight: public LightSource 
{
public:
    DiffuseLight(): LightSource() {}
    virtual ~DiffuseLight() {};

    virtual float getArea() const { return size_.x*size_.y; }
    /**
     * Get radiant flux (color) of light source.
     * @see setPower
     * @return Radiant flux in watt.
     */
    virtual vec3 getPower() const { return getIntensity()*getArea(); }


    LightSourceType::Enum getLightSourceType() const { return LightSourceType::LIGHT_AREA; }

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
    vec3 intensity_; // Color of light source, flux density per solid angle (given in watt per steradian W*s*r^-1)
    
};

} // namespace inviwo

#endif // IVW_DIFFUSE_LIGHT_H