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

#ifndef RAY_SPHERE_INTERSECTION_CL
#define RAY_SPHERE_INTERSECTION_CL

// Intersects a ray with a sphere.
// If intersecting, t1 is the point of intersection along the ray
bool raySphereIntersection(const float3 sphereCenter, const float radius, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) {
    
    float3 m = o-sphereCenter;
    float b = dot(m, d);
    float c = dot(m, m)-radius*radius;
    // Exit if ray origin is outside of sphere and pointing away from sphere
    if(c > 0.f && b > 0.f) return false;

    float discr = b*b-c;
    // Negative discriminat means that ray misses sphere
    if(discr < 0.f) return false;

    // Ray intersects sphere, compute first intersection point (smallest t1)
    *t1 = -b - native_sqrt(discr);
    // If t is negative, ray started inside sphere, so we clamp it to zero
    if( *t1 < 0.f ) *t1 = 0.f;

    return true;
}


#endif
