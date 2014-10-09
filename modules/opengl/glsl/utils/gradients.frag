/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/
#ifndef IVW_GRADIENTS_GLSL
#define IVW_GRADIENTS_GLSL

/********** For selected channel **********/

// Compute world space gradient using forward difference: f' = ( f(x+h)-f(x) ) / h
mat4x3 gradientForwardDiff(vec4 intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) forward differences
    // Value at f(x+h)
    mat3x4 fDs;
	// Moving a fixed distance h along each xyz-axis in world space, which correspond to moving along
    // three basis vectors in texture space. 
    // This will be the minimum world space voxel spacing for volumes with orthogonal basis function.
    fDs[0] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[0]);
    fDs[1] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[1]);
    fDs[2] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[2]);
    // f' = ( f(x+h)-f(x) ) / h
	return transpose(fDs - mat3x4(intensity,intensity,intensity)) / volumeParams.worldSpaceGradientSpacing_;
}

// Compute world space gradient using central difference: f' = ( f(x+h)-f(x-h) ) / 2*h
mat4x3 gradientCentralDiff(vec4 intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    // Of order O(h^2) central differences
    mat3x4 cDs; 
    // Value at f(x+h)
    cDs[0] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] = getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[2]);
    // Value at f(x-h)
    cDs[0] -= getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] -= getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] -= getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[2]);
    // f' = ( f(x+h)-f(x-h) ) / 2*h
    return transpose(cDs) / (2.0*volumeParams.worldSpaceGradientSpacing_);
}

// Compute world space gradient using backward difference: f' = ( f(x)-f(x-h) ) / h
mat4x3 gradientBackwardDiff(vec4 intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    // Of order O(h^2) backward differences
    mat3x4 fDs;
	// Value at f(x-h)
    fDs[0] = getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[0]);
    fDs[1] = getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[1]);
    fDs[2] = getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[2]);
    // f' = ( f(x)-f(x-h) ) / h
    return transpose(mat3x4(intensity,intensity,intensity)-fDs) / volumeParams.worldSpaceGradientSpacing_;
}

// Compute world space gradient using higher order central difference: f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*h
mat4x3 gradientCentralDiffH(vec4 intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^4) central differences
    mat3x4 cDs;
     // Value at 8.f(x+h)
    cDs[0] = 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] = 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] = 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[2]);
    // Value at 8.f(x-h)
    cDs[0] -= 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] -= 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] -= 8.0 * getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[2]);
    // Value at -f(x+2h)
    cDs[0] -= getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] -= getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] -= getNormalizedVoxel(volume, volumeParams, samplePos + volumeParams.textureSpaceGradientSpacing_[2]);
    // Value at f(x+2h)
    cDs[0] += getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[0]);
    cDs[1] += getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[1]);
    cDs[2] += getNormalizedVoxel(volume, volumeParams, samplePos - volumeParams.textureSpaceGradientSpacing_[2]);
    // f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*h
    return transpose(cDs)/(12.0*volumeParams.worldSpaceGradientSpacing_);
}


/********** For the first channel (red) **********/

vec3 gradientForwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return gradientForwardDiff(vec4(intensity), volume, volumeParams, samplePos)[0];
}
vec3 gradientCentralDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return gradientCentralDiff(vec4(intensity), volume, volumeParams, samplePos)[0];
}
vec3 gradientBackwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return gradientBackwardDiff(vec4(intensity), volume, volumeParams, samplePos)[0];
}
vec3 gradientCentralDiffH(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return gradientCentralDiffH(vec4(intensity), volume, volumeParams, samplePos)[0];
}

#endif // IVW_GRADIENTS_GLSL

