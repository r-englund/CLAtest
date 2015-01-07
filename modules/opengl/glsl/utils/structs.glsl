/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

struct CameraParameters {
   mat4 worldToView; // Equivalent to view
   mat4 viewToWorld; // Equivalent to viewInverse
   mat4 worldToClip; // Equivalent to viewProjection
   mat4 clipToWorld; // Equivalent to viewProjectionInverse
   float nearPlane;  // zNear
   float farPlane;   // zFar
};
// Convenience functions to retrieve camera parameters
/* lookAt(vec3 eye, vec3 center, vec3 up) {
    vec3 cameraDir(normalize(center - eye));
    vec3 right(normalize(cross(cameraDir, up)));
    up = (cross(right, cameraDir));
    vec3 lookFrom(dot(right, eye), dot(up, eye), -dot(cameraDir, eye));
    [  right[0], up[0], -cameraDir[0], -lookFrom[0] ] 
    [  right[1], up[1], -cameraDir[1], -lookFrom[1] ] 
    [  right[2], up[2], -cameraDir[2], -lookFrom[2] ] 
    [         0,     0,         0,       1     ] */

vec3 right(in CameraParameters camera) { return camera.worldToView[0].xyz; }
vec3 up(in CameraParameters camera) { return camera.worldToView[1].xyz; }
vec3 direction(in CameraParameters camera) { return -camera.worldToView[2].xyz; }
vec3 position(in CameraParameters camera) { return -camera.worldToView[3].xyz; }



struct LIGHT_PARAMETERS {
    vec3 position_; 
    vec3 ambientColor_;
    vec3 diffuseColor_; 
    vec3 specularColor_;
    int specularExponent_;
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


struct MODEL_PARAMETERS {
    mat4 modelToWorldMatrix_;
    mat3 modelToWorldNormalMatrix_;
};



// Depricated...

struct CAMERA_PARAMETERS {
    mat4 viewMatrix_;
    mat4 projectionMatrix_; // view to clip
    mat4 worldToClipMatrix_; // view*projection
    vec3 cameraPosition_;
	float zNear_;
	float zFar_;
};


#endif // IVW_STRUCTS_GLSL