
#ifndef TRANSFORMATIONS_CL
#define TRANSFORMATIONS_CL

// Assume that the fourth component in x is 1
float3 transformPoint(const float16 m, const float3 x)
{
     return (float3)(dot(m.s012, x)+m.s3, dot(m.s456, x)+m.s7, dot(m.s89a, x)+m.sb);
}
float4 transformPoint4(const float16 m, const float4 x)
{
     return (float4)(dot(m.s0123, x), dot(m.s4567, x), dot(m.s89ab, x), dot(m.scdef, x));
}
// Transform and project
float3 transformPointW(const float16 m, const float3 x)
{
    return (float3)(dot(m.s012, x)+m.s3, dot(m.s456, x)+m.s7, dot(m.s89a, x)+m.sb)/(dot(m.scde, x)+m.sf);
}
// Transform and project
float4 transformPoint4W(const float16 m, const float4 x)
{
    float4 point = (float4)(dot(m.s0123, x), dot(m.s4567, x), dot(m.s89ab, x), dot(m.scdef, x));
    point /= point.w;
    return point;
}
// Transform a direction (does not apply translation)
float3 transformVector(const float16 m, const float3 x)
{
    return (float3)(dot(m.s012, x), dot(m.s456, x), dot(m.s89a, x));
}

float3 translatePoint(const float16 m, const float3 x)
{
     return x+m.s37b;
}


// Encode xyz direction into theta, phi angles
// Assumes normalized direction, i.e. r = 1
float2 encodeDirection(float3 dir) {
    float phi = atan2(dir.y, dir.x);
    //if ( !isfinite(phi) ) {
        //if(dir.y < 0.f) phi = -0.5f*M_PI;
        //else            phi = 0.5f*M_PI;
    //}
    // Important: clamp dir.z to avoid NaN
    float theta = acos(clamp(dir.z,-1.f, 1.f));

    return (float2)(theta, phi);
}

// Decode theta, phi angles to xyz direction 
float3 decodeDirection(float2 angles) {
    // Evaluated three different ways of computing this.
    // The first one is by far the fastest.
    float2 cosAngles = native_cos(angles);
    float2 sinAngles = native_sin(angles);
    return (float3)(sinAngles.x*cosAngles.y,
                sinAngles.x*sinAngles.y,
                cosAngles.x);
    // Option 2 (slightly faster than option 3):
    //float2 cosAngles;
    //// Calculate both sine and cosine at once
    //float2 sinAngles = sincos(angles, &cosAngles);
    // Option 3:
    //return (float3)(native_sin(angles.x)*native_cos(angles.y),
    //                native_sin(angles.x)*native_sin(angles.y),
    //                native_cos(angles.x));
}


#endif // TRANSFORMATIONS_CL