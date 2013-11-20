#ifndef LIGHTSOURCE_CL
#define LIGHTSOURCE_CL

typedef enum LightSourceType {
    LIGHT_AREA = 0,
    LIGHT_CONE,
    LIGHT_POINT,
    LIGHT_DIRECTIONAL
} LightSourceType;

// Note that largest variables should be placed first 
// in order to ensure struct size
typedef struct PhotonLightSource {
    float16 tm; // Transformation matrix from local to world coordinates
    float3 radiance; // Note that float3 occupies same space as float4
    float2 size; // width, height of light source, used by area light
    int type; // LightSourceType, use integer to handle size of struct easier
    float area; // area of light source
    float cosFOV;  // cos( (field of view)/2 ), used by cone light
    
    int padding[7];
} PhotonLightSource;

#endif // LIGHTSOURCE_CL