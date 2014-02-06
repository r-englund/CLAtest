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

vec3 gradientForwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) forward differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    // Value at f(x+h)
    vec3 fDs;
    fDs.x = getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).r;
    fDs.y = getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).r;
    fDs.z = getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x+h)-f(x) ) / voxelSpacing
    return (fDs-intensity)/(voxelSpacing);
}

vec3 gradientCentralDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) central differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    vec3 cDs;
    // Value at f(x+h)
    cDs.x = getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).r;
    cDs.z = getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Value at f(x-h)
    cDs.x = cDs.x - getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = cDs.y - getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).r;
    cDs.z = cDs.z - getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x+h)-f(x-h) ) / 2*voxelSpacing
    return (cDs)/(2.0*voxelSpacing);
}

vec3 gradientBackwardDiff(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^2) backward differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    // Value at f(x-h)
    vec3 fDs;
    fDs.x = getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).r;
    fDs.y = getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).r;
    fDs.z = getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( f(x)-f(x-h) ) / voxelSpacing
    return (intensity-fDs)/(voxelSpacing);
}

//Higher order gradients

vec3 gradientCentralDiffH(float intensity, VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    //Of order O(h^4) central differences
    vec3 voxelSpacing = volumeParams.dimensionsRCP_;
    vec3 cDs;
    // f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*h
    // Value at 8.f(x+h)
    cDs.x = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(0.0, voxelSpacing.y, 0.0)).r;
    cDs.z = 8.0 * getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Value at 8.f(x-h)
    cDs.x = cDs.x - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = cDs.y - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(0.0, voxelSpacing.y, 0.0)).r;
    cDs.z = cDs.z - 8.0 * getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, voxelSpacing.z)).r;
    // Value at -f(x+2h)
    cDs.x = cDs.x - getVoxel(volume, volumeParams, samplePos+vec3(2.0*voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = cDs.y - getVoxel(volume, volumeParams, samplePos+vec3(0.0, 2.0*voxelSpacing.y, 0.0)).r;
    cDs.z = cDs.z - getVoxel(volume, volumeParams, samplePos+vec3(0.0, 0.0, 2.0*voxelSpacing.z)).r;
    // Value at f(x+2h)
    cDs.x = cDs.x + getVoxel(volume, volumeParams, samplePos-vec3(2.0*voxelSpacing.x, 0.0, 0.0)).r;
    cDs.y = cDs.y + getVoxel(volume, volumeParams, samplePos-vec3(0.0, 2.0*voxelSpacing.y, 0.0)).r;
    cDs.z = cDs.z + getVoxel(volume, volumeParams, samplePos-vec3(0.0, 0.0, 2.0*voxelSpacing.z)).r;
    // Note that this computation is performed in texture space (by using voxelSpacing)
    // f' = ( -f(x+2h)+8.f(x+h)-8.f(x-h)+f(x-2h) ) / 12*voxelSpacing
    return (cDs)/(12.0*voxelSpacing);
}
