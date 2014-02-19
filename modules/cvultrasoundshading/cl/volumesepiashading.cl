/**********************************************************************
 * Copyright (C) 2014 ContextVision AB
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

#include "gradients.cl" 
#include "samplers.cl"
#include "shading/shading.cl"  
 
__constant float REF_SAMPLING_INTERVAL = 150.f;
     
__kernel void volumeSepiaShading(read_only image3d_t volume
                        , read_only image2d_t entryPoints
                        , read_only image2d_t exitPoints
                        , read_only image2d_t transferFunction 
                        , float stepSize
                        , float3 voxelSpacing
                        , float3 lightDirection
                        , float3 lightPower
                        , ShadingType shadingType
                        , float4 materialSpecular
                        , float4 material
                        , write_only image2d_t output) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));      
    if( any(globalId >= get_image_dim(output)) ) {
        return;
    }

    float4 entry = read_imagef(entryPoints, smpUNormNoClampNearest, globalId);  
    
    float4 result = (float4)(0.f); 
    float4 exit = read_imagef(exitPoints, smpUNormNoClampNearest, globalId);   
    float3 direction = exit.xyz - entry.xyz;   
    float tEnd = fast_length(direction);
    direction = fast_normalize(direction);
    float3 p = entry.xyz;
    float t = 0.0f; 

    float volumeSample;
    float extinction = 0.f;  
    while(t < tEnd) {
        float3 pos = entry.xyz+t*direction; 
        volumeSample = read_imagef(volume, smpNormClampEdgeLinear, as_float4(pos)).x; 
        // xyz == diffuse color, w = absorption
        float4 materialDiffuse = read_imagef(transferFunction, smpNormClampEdgeLinear, (float2)(volumeSample, 0.5f));

        float4 materialSpecular = 1.f;
        float3 gradient = calcGradient(as_float4(pos), volume, as_float4(voxelSpacing)).xyz;

        gradient = normalize(gradient);


        float3 f = applyShading(-direction, -lightDirection, materialDiffuse.xyz, materialSpecular.xyz, material, gradient, shadingType);
        // Taylor expansion approximation
        materialDiffuse.w = 1.f - native_powr(1.f - materialDiffuse.w, stepSize * REF_SAMPLING_INTERVAL);
		result.xyz = result.xyz + (1.f - result.w) * materialDiffuse.w * f * lightPower;
        result.w = result.w + (1.f - result.w) * materialDiffuse.w;	

        t += stepSize;   
 
    }
         
    write_imagef(output, globalId,  result);      
  
}
  