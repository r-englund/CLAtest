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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#include "samplers.cl"
#include "transformations.cl"
#include "intersection/rayboxintersection.cl"

__kernel void entryexitpoints(float16 NDCToTextureMat
							, float4 camPosInTextureSpace
							, float2 nearFarPlaneDist
							, write_only image2d_t entryPoints
							, write_only image2d_t exitPoints) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));      
    if( any(globalId >= get_image_dim(entryPoints)) ) {
        return;
    }

	float2 normalizedScreenCoord = (float2)(get_global_id(0)+0.5f, get_global_id(1)+0.5f)/convert_float2(get_image_dim(entryPoints));
	float3 normalizedDeviceCoord = (float3)(2.0f*normalizedScreenCoord-1.0f, 1.0f);
	// convert the ndc back to the volume texture coordinates
	float3 dir = normalize(transformPoint(NDCToTextureMat, normalizedDeviceCoord));

	BBox bbox; bbox.pMin = (float3)(0.f); bbox.pMax = (float3)(1.f);
	float t0 = nearFarPlaneDist.x; float t1 = nearFarPlaneDist.y;
	bool hit = rayBoxIntersection(bbox, camPosInTextureSpace.xyz, dir, &t0, &t1);
    
	if(hit) {
		write_imagef(entryPoints, globalId,  (float4)(camPosInTextureSpace.xyz+t0*dir, 1.f));     
		write_imagef(exitPoints, globalId,  (float4)(camPosInTextureSpace.xyz+t1*dir,1.f));    
	} else {
		write_imagef(entryPoints, globalId,  (float4)(0.f));     
		write_imagef(exitPoints, globalId,  (float4)(0.f));    
	}
	 
  
}
  