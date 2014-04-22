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

__constant float REF_SAMPLING_INTERVAL = 150.f;
#define ERT_THRESHOLD 1.0

__kernel void raycaster(read_only image3d_t volume
                        , read_only image2d_t entryPoints
                        , read_only image2d_t exitPoints
                        , read_only image2d_t transferFunction 
                        , float stepSize
                        , uint3 volumeDimension
                        , write_only image2d_t output) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));      

    if (any(globalId >= get_image_dim(output))) {
        return;
    }
    float4 entry = read_imagef(entryPoints, smpUNormNoClampNearest, globalId);  
    float4 exit = read_imagef(exitPoints, smpUNormNoClampNearest, globalId);
    float3 direction = exit.xyz - entry.xyz;   
    float tEnd = fast_length(direction);
    float4 result = (float4)(0.f); 
    if(tEnd > 0.f) {     
        direction = fast_normalize(direction);
        float tIncr = min(tEnd, 1.f/(stepSize*length(direction*convert_float3(volumeDimension)))); 
        // Start integrating at the center of the bins
        float t = 0.5f*tIncr; 
        float4 emissionAbsorption;
        while(t < tEnd) {
            float3 pos = entry.xyz+t*direction;
            float volumeSample = read_imagef(volume, smpNormClampEdgeLinear, as_float4(pos)).x; 
            // xyz == emission, w = absorption
            emissionAbsorption = read_imagef(transferFunction, smpNormClampEdgeLinear, (float2)(volumeSample, 0.5f));
            // Taylor expansion approximation
            float opacity = 1.f - native_powr(1.f - emissionAbsorption.w, tIncr * REF_SAMPLING_INTERVAL);
			result.xyz = result.xyz + (1.f - result.w) * opacity * emissionAbsorption.xyz;
            result.w = result.w + (1.f - result.w) * opacity;	

            if (result.w > ERT_THRESHOLD) t = tEnd;   
            else t += tIncr;   
        }
        // Remove the last part of the integration that was too much.
        float dt = tEnd-(t-0.5f*tIncr);
        float opacity = 1.f - native_powr(1.f - emissionAbsorption.w, dt * REF_SAMPLING_INTERVAL);
		result.xyz = result.xyz + (1.f - result.w) * opacity * emissionAbsorption.xyz;
        result.w = result.w + (1.f - result.w) * opacity;	
    }
         
    write_imagef(output, globalId,  result);     
  
}
  