#ifndef IVW_BASELIGHT_H
#define IVW_BASELIGHT_H


#include <inviwo/core/common/inviwo.h>


namespace inviwo {

namespace LightSourceType {
    enum Enum {            
        LIGHT_AREA = 0,
		LIGHT_CONE,
        LIGHT_POINT,
        LIGHT_DIRECTIONAL
    };
}

class BaseLightSource {
public:
	BaseLightSource() {};
	virtual ~BaseLightSource() {};
    mat4 tm; // Transformation matrix from local to world coordinates
    vec4 radiance; // Color of light source
    LightSourceType::Enum type;
    float area;
	vec2 size; // width, height
    float fov; // Field of view in radians
    vec3 lightdir;
};

// Data type that can be transfered to OpenCL device
// Must be same as modules/opencl/cl/light.cl
// Note that largest variables should be placed first 
// in order to ensure struct size
typedef struct {
    mat4 tm; // Transformation matrix from local to world coordinates
    vec4 radiance; // cl_float3 == cl_float4
    vec2 size; // width, height
    int type; // LightSourceType, use integer to handle size of struct easier
    float area; // area of light source
    float cosFOV; // cos( (field of view)/2 ), used by cone light

    int padding[7]; // OpenCL requires sizes that are power of two (32, 64, 128 and so on)
}PackedLightSource;

// Transform a BaseLightSource to PhotonLightSourceCL 
PackedLightSource baseLightToPackedLight(const BaseLightSource* lightsource, float radianceScale);

// Calculate how many samples to take from each light source
uvec2 getSamplesPerLight(uvec2 nSamples, int nLightSources);


} // namespace inviwo

#endif // IVW_BASELIGHT_H
