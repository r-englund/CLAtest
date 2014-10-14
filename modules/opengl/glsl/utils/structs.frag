/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_STRUCTS_GLSL
#define IVW_STRUCTS_GLSL

#define VOLUME_TYPE sampler3D
#define TEXTURE_TYPE sampler2D

struct SHADING_PARAMETERS {
    vec3 lightPosition_;
    vec3 lightColorAmbient_;
    vec3 lightColorDiffuse_;
    vec3 lightColorSpecular_;
    int lightSpecularExponent_;
};

struct TEXTURE_PARAMETERS {
    vec2 dimensions_;
    vec2 dimensionsRCP_;
};

struct VOLUME_PARAMETERS {
    vec3 dimensions_;
    vec3 dimensionsRCP_;
    mat4 worldToTexture_;
    mat4 textureToWorld_;
    mat3 textureSpaceGradientSpacing_; // Maximum distance we can without going outside a voxel from the center of it (half of minimum voxel spacing for volumes with orthogonal basis)
    float worldSpaceGradientSpacing_;  // Gradients are returned in world space and hence we need this to get correct length
    float formatScaling_; //Reversed, meaning scaling = (1-bitScaling_), as 0 is default for single uniforms.
    float formatOffset_;
    float signedFormatScaling_; //Reversed, meaning scaling = (1-bitScaling_), as 0 is default for single uniforms.
    float signedFormatOffset_;
};

struct CAMERA_PARAMETERS {
    mat4 viewMatrix_;
    vec3 cameraPosition_;
	float zNear_;
	float zFar_;
};

#endif // IVW_STRUCTS_GLSL