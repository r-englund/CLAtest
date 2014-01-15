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

#ifndef IVW_BASELIGHT_H
#define IVW_BASELIGHT_H

#include <inviwo/core/common/inviwocoredefine.h>
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
// TODO: Change/add transformation and size information to meters instead of texture space.

class IVW_CORE_API LightSource {
public:
	LightSource() {};
	virtual ~LightSource() {};

    virtual float getArea() const = 0;

    /**
     * Get radiant flux (color) of light source.
     * @return Radiant flux in watt.
     */
    virtual vec3 getPower() const = 0;

    const mat4& getObjectToTexture() const { return objectToTextureSpaceMatrix_; }
    void setObjectToTexture(const mat4& m) { objectToTextureSpaceMatrix_ = m; }

    virtual LightSourceType::Enum getLightSourceType() const = 0;

    /**
     * Return field of view in radians.
     * 
     * @return Field of view in radians
     */
    float getFieldOfView() const { return fieldOfView_; }

    void setFieldOfView(float FOVInRadians) { fieldOfView_ = FOVInRadians; }

    /**
     * Get normalized general direction of light source. 
     * 
     * @return Normalized direction of light source.
     */
    const vec3& getLightDirection() const { return lightDirection_; }

    /**
     * Set normalized direction of light source.
     * 
     * @param direction Normalized direction of light source.
     */
    void setLightDirection(const vec3& direction) { lightDirection_ = direction; }

    /**
     * Get width and height in texture space.
     * 
     * @return 
     */
    const vec2& getSize() const { return size_; }

    /**
     * Set width and height in texture space.
     * 
     * @param newSize 
     */
    void setSize(const vec2& newSize) { size_ = newSize; }
protected:

    mat4 objectToTextureSpaceMatrix_; // Transformation matrix from object to texture space coordinates
    float fieldOfView_; // Field of view in radians
    vec3 lightDirection_;
    vec2 size_; // width, height in texture space
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
IVW_CORE_API PackedLightSource baseLightToPackedLight(const LightSource* lightsource, float radianceScale);

// Calculate how many samples to take from each light source. 
// x component contains the amount of samples to take in x and y dimension
// y component is the number of samples taken for each light source (x*x)
IVW_CORE_API uvec2 getSamplesPerLight(uvec2 nSamples, int nLightSources);


} // namespace inviwo

#endif // IVW_BASELIGHT_H
