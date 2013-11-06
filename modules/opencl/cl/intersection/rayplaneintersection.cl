#ifndef RAY_PLANE_INTERSECTION_CL
#define RAY_PLANE_INTERSECTION_CL

float scalarTripleProduct(float3 u, float3 v, float3 w) {
    return dot(cross(u, v), w);
}

// Intersection test of a ray with a plane. 
// If intersecting, t1 is the point of intersection along the ray
bool rayPlaneIntersection(const float3 planePos, const float3 planeNormal, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) { 
    // http://en.wikipedia.org/wiki/Line-plane_intersection
    // http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-7-intersecting-simple-shapes/ray-plane-and-ray-disk-intersection/
    float denom = dot(d, planeNormal);
    // If denominator == 0, then segment is parallel to plane
    // Otherwise, it points to or away from the plane
    if( fabs(denom) > 1e-6 ){
        float numerator = dot((planePos - o), planeNormal);

        *t1 =  numerator / denom;
        return *t1 >= 0.f;  
        
    } 
    return false;

  
}

// Intersects a ray with a Quad.
// If intersecting, t1 is the point of intersection along the ray
// C-D
// | |
// A-B
bool rayQuadIntersection(const float3 A, const float3 B, const float3 C, const float3 D, const float3 o, const float3 d, float * __restrict t0, float* __restrict t1) { 

    // Test if intersection is within bounds
    float3 oa = A - o;
    float3 ob = B - o;
    float3 oc = C - o;

    // Determine which triangle to test against by testing diagonal first
    float3 m = cross(oc, d);
    float v = dot(oa, m);
    float3 hitPoint;
    if( v >= 0.f ) {
        // Test intersection against triangle abc
        float u = -dot(ob, m);
        if(u < 0.f) return false;

        float w = scalarTripleProduct(d, ob, oa);
        if( w < 0.f ) return false;

        // Compute intersection point
        float denom = 1.f /(u+v+w);
        u *= denom; v *= denom; w *= denom;
        hitPoint = u*A+v*B+w*C;
    } else {
        float3 od = D - o;
        float u = dot(od, m);
        if(u < 0.f) return false;

        float w = scalarTripleProduct(d, oa, od);
        if( w < 0.f) return false;

        v = -v;
        // Compute intersection point
        float denom = 1.f /(u+v+w);
        u *= denom; v *= denom; w *= denom;
        hitPoint = u*A+v*D+w*C;
        
    }
    *t1 = length(hitPoint-o);
    return true;

}

#endif
