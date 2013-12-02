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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_COLORCONVERSION_H
#define IVW_COLORCONVERSION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

IVW_CORE_API inline vec3 hsv2rgb(const vec3 &hsv)
{
    float hue = hsv.x;
    float sat = hsv.y;
    float val = hsv.z;

    float x = 0.f, y = 0.f, z = 0.f;

    if (hue == 1.f) 
        hue = 0.f;
    else 
        hue *= 6.f;

    int i = int(glm::floor(hue));
    float f = hue-i;
    float p = val*(1-sat);
    float q = val*(1-(sat*f));
    float t = val*(1-(sat*(1-f)));

    switch (i) {
        case 0: x = val; y = t; z = p; break;
        case 1: x = q; y = val; z = p; break;
        case 2: x = p; y = val; z = t; break;
        case 3: x = p; y = q; z = val; break;
        case 4: x = t; y = p; z = val; break;
        case 5: x = val; y = p; z = q; break;
    }

    return vec3(x,y,z);
}

IVW_CORE_API inline vec3 rgb2hsv(const vec3 &rgb)
{
    const float &x = rgb.r;
    const float &y = rgb.g;
    const float &z = rgb.b;

    float max	 = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    float min	 = (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
    float range = max - min;
    float val	 = max;
    float sat   = 0.f;
    float hue   = 0.f;

    if (max != 0.f)   
        sat = range/max;

    if (sat != 0.f){
        float h;

        if (x == max)	
            h = (y - z) / range;
        else if (y == max)	
            h = 2.f + (z - x) / range;
        else		
            h = 4.f + (x - y) / range;

        hue = h/6.f;

        if (hue < 0.f)
            hue += 1.f;
    }
    return vec3(hue,sat,val);
}

#endif // IVW_COLORCONVERSION_H
