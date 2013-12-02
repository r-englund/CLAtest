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

#ifndef RAY_BOX_INTERSECTION_CL
#define RAY_BOX_INTERSECTION_CL

#include "datastructures/bbox.cl"

// Test intersection between ray and axis-aligned bounding box.
// Returns distance to first intersection in t0 and second intersection in t1
// If ray is inside the box, t0 will be 0 if initial value is 0, if it is negative it may contain
// the first hit point on the line (along the negative direction);
// t1 will contain the hitpoint 
bool rayBoxIntersection(const BBox bbox, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) {
    //*t0 = 0.f; // set to -FLT_MAX to get first hit on line (also in negative direction, i.e. behind the origin)
    //*t1 = MAXFLOAT;

    // Ray is parallel to slab?
    // If nan is not supported this needs to be checked
    //if( any((fabs(d) < 1e-6) & (o < (bbox).pMin | o > (bbox).pMax)) ) // Check if parallel and outside of box
    //    return false;


    float3 invDir = 1.f/d;
    float3 tNearV = ( (bbox).pMin-o ) * invDir;
    float3 tFarV = ( (bbox).pMax-o ) * invDir;
    float3 tNear = fmin(tNearV, tFarV);
    float3 tFar = fmax(tNearV, tFarV); 
    
    *t0 = fmax(*t0, fmax( fmax(tNear.x, tNear.y), tNear.z ) );
    *t1 = fmin(*t1, fmin( fmin(tFar.x, tFar.y), tFar.z ) );
    // Note that check with FLT_MAX is crucial for stability
    return (*t0 < *t1) && (*t1 < FLT_MAX);


}
#endif
