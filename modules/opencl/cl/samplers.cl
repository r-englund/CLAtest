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
 * Contact: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef SAMPLERS_CL
#define SAMPLERS_CL

__constant sampler_t smpUNormNoClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
__constant sampler_t smpUNormClampNearest = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;
__constant sampler_t smpNormClampLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
__constant sampler_t smpNormClampEdgeLinear = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_LINEAR;
__constant sampler_t smpNormClampEdgeNearest = CLK_NORMALIZED_COORDS_TRUE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;

/*
 * Get voxel data with linear interpolation, coordinate in [0 1]^3
 */
float getVoxel(read_only image3d_t volume, float4 pos) {
#ifdef SCALE_VOLUME_DATA
    // Scale 12-bit data
    return read_imagef(volume, smpNormClampEdgeLinear, pos).x * VOLUME_FORMAT_SCALING;  
#else 
    return read_imagef(volume, smpNormClampEdgeLinear, pos).x;  
#endif
}
/*
 * Get voxel data in unnormalized coordinates in [0 get_image_dim(volume)-1]
 */
float getVoxelUnorm(read_only image3d_t volume, int4 pos) {
#ifdef SCALE_VOLUME_DATA
    // Scale 12-bit data
    return read_imagef(volume, smpUNormNoClampNearest, pos).x * VOLUME_FORMAT_SCALING;  
#else 
    return read_imagef(volume, smpUNormNoClampNearest, pos).x;  
#endif
}
/*
 * Get voxel data with linear interpolation and apply offset and scaling, coordinate in [0 1]^3
 */
float getVoxelS(read_only image3d_t volume, float4 pos, float2 offsetAndScaling) {
    return (read_imagef(volume, smpNormClampEdgeLinear, pos).x + offsetAndScaling.x) * offsetAndScaling.y;  
}

/*
 * Get voxel data in unnormalized coordinates [0 get_image_dim(volume)-1] and apply offset and scaling
 */
float getVoxelUnormS(read_only image3d_t volume, int4 pos, float2 offsetAndScaling) {
    return (read_imagef(volume, smpUNormNoClampNearest, pos).x + offsetAndScaling.x) * offsetAndScaling.y;  
}

#endif // SAMPLERS_CL
