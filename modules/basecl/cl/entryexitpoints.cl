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
  