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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#include "samplers.cl" 

//#define VOLUME_OUTPUT_UINT_TYPE

#ifdef SUPPORTS_VOLUME_WRITE
#pragma OPENCL_EXTENSION cl_khr_3d_image_writes : enable
#endif

__kernel void volumeMaxKernel(read_only image3d_t volumeIn, float2 volumeDataScaling
#ifdef SUPPORTS_VOLUME_WRITE
    , write_only image3d_t volumeOut
#else
#ifdef VOLUME_OUTPUT_UINT_TYPE
    , __global uint* volumeOut
#else
    , __global uchar* volumeOut
#endif
    //, __global float* volumeOut
#endif    
    
    , int4 outDim
    , int4 region
    ) 
{
    //output image pixel coordinates 
    int3 globalId = (int3)(get_global_id(0), get_global_id(1), get_global_id(2));  

    
    if (any(globalId>=outDim.xyz)) {
        return;
    }
    float maxVal = 0;
    int4 startCoord = (int4)(globalId*region.xyz, 0);
    for (int z = 0; z < region.z; ++z) {
        for (int y = 0; y < region.y; ++y) {
            for (int x = 0; x < region.x; ++x) {
                maxVal = max(maxVal, getVoxelUnormS(volumeIn, startCoord+(int4)(x, y, z, 0), volumeDataScaling));
            }
        }
    }
#ifdef SUPPORTS_VOLUME_WRITE
    #ifdef VOLUME_UINT_TYPE
        write_imageui(volumeOut, (int4)(globalId, 0), (uint4)(maxVal)); 
    #else
        write_imagef(volumeOut, (int4)(globalId, 0), (float4)(convert_float4(maxVal))); 
    #endif
#else
    int threadId = get_global_id(0) + get_global_id(1)*outDim.x+get_global_id(2)*outDim.x*outDim.y;
    //volumeOut[threadId] = maxVal;
    #ifdef VOLUME_OUTPUT_UINT_TYPE
        volumeOut[threadId] = convert_uint_sat_rte(maxVal*4294967295.f);
    #else
        // The preferred method for conversions from floating-point values to normalized integer
        volumeOut[threadId] = convert_uchar_sat_rte(maxVal*255.f);
    #endif
#endif
}