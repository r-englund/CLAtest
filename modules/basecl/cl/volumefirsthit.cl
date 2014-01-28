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


__kernel void volumeFirstHit(read_only image3d_t volume
                        , read_only image2d_t entryPoints
                        , read_only image2d_t exitPoints
                        , read_only image2d_t transferFunction 
                        , float stepSize
                        , write_only image2d_t output) 
{
    int2 globalId = (int2)(get_global_id(0), get_global_id(1));      
    //uint3 volumeDimension = (uint3)(128,128, 128); 
    if( any(globalId >= get_image_dim(output)) ) {
        return;
    }
    float4 entry = read_imagef(entryPoints, smpUNormNoClampNearest, globalId);  
    float4 exit = read_imagef(exitPoints, smpUNormNoClampNearest, globalId);   

    float3 direction = exit.xyz - entry.xyz;   
    float tEnd = length(direction);
    direction = normalize(direction);

    float t = 0.0f; 
    float absorption = 0.f; 
    float3 pos;
    while(t <= tEnd && absorption == 0.f) {
        pos = entry.xyz+t*direction;
        float volumeSample = read_imagef(volume, smpNormClampEdgeLinear, as_float4(pos)).x; 
        // xyz == emission, w = absorption
        absorption = read_imagef(transferFunction, smpNormClampEdgeLinear, (float2)(volumeSample, 0.5f)).w;
        t += stepSize;    
    }
         
    write_imagef(output, globalId,  (float4)(pos, entry.w));     
  
}
  