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
