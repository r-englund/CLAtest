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

#ifndef RAY_DISC_INTERSECTION_CL
#define RAY_DISC_INTERSECTION_CL

// Intersects a ray with a disc.
// If intersecting, t1 is the point of intersection along the ray
bool rayDiscIntersection(const float3 discCenter, const float3 discNormal, float radius, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) { 
    float denom = dot(d, discNormal);
    // If denominator <= 0, then segment is parallel to or points away from the plane
    if( denom <= 0.f ) return false;
    float numerator = dot((discCenter - o), discNormal);
  
    *t1 =  numerator / denom;
    return distance(o+(*t1)*d, discCenter) <= radius;
}

#endif
