#ifndef RAY_SPHERE_INTERSECTION_CL
#define RAY_SPHERE_INTERSECTION_CL

// Intersects a ray with a sphere.
// If intersecting, t1 is the point of intersection along the ray
bool raySphereIntersection(const float3 sphereCenter, const float radius, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) {
    
    float3 m = o-sphereCenter;
    float b = dot(m, d);
    float c = dot(m, m);
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
