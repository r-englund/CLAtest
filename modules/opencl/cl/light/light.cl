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

#ifndef LIGHT_CL
#define LIGHT_CL

#include "datastructures/lightsource.cl"
#include "shading/shadingmath.cl"
#include "transformations.cl"



int getLightSourceId(const int2 nPhotonsPerLight, const int nLightSources) {
    return (get_global_id(1)*get_global_size(0)+get_global_id(0))/nPhotonsPerLight.y;
}



float2 getLightSourceUniformUV(const int2 nPhotonsPerLight, const int nLightSources) {
    int globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    int lightSourceId =  getLightSourceId(nPhotonsPerLight, nLightSources);
 
    int lightSourceOffset = lightSourceId*nPhotonsPerLight.y;


    int lightSampleIndex = globalId-lightSourceOffset;
    int2 photonId = (int2)(lightSampleIndex%nPhotonsPerLight.x, lightSampleIndex/nPhotonsPerLight.x);

    return (float2)((convert_float2(photonId)+0.5f)/convert_float(nPhotonsPerLight.x));
}
float2 getLightSourceStratifiedUV(const int2 nPhotonsPerLight, const int nLightSources, const float2 randNum) {
    int globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);

    int lightSourceId =  getLightSourceId(nPhotonsPerLight, nLightSources);

    int lightSourceOffset = lightSourceId*nPhotonsPerLight.y;


    int lightSampleIndex = globalId-lightSourceOffset;
    // Calculate integer xy coordinate on light
    int2 photonId = (int2)(lightSampleIndex%nPhotonsPerLight.x, lightSampleIndex/nPhotonsPerLight.x);

    return (float2)((convert_float2(photonId)+randNum)/convert_float(nPhotonsPerLight.x));
}

void sampleLights(__global LightSource const * __restrict lightSources, const int2 nPhotonsPerLight,
                  const int nLightSources, const float2 uv, float3* __restrict origin, float3* __restrict wi, float3* __restrict power, float* __restrict pdf, float3 rndNum, const BBox bbox) {
 
    uint globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    int lightSourceId = getLightSourceId(nPhotonsPerLight, nLightSources);

    LightSource lightSource = lightSources[lightSourceId];
    
    if( lightSource.type == LIGHT_POINT ) {
        float3 localOrigin = (float3)(0.f);
        *origin = transformPoint(lightSource.tm, localOrigin);
        float3 sphereSample = uniformSampleSphere(uv);
        *wi = -sphereSample;

        *pdf = uniformSpherePdf();
        *power = lightSource.radiance / *pdf;
    } 
    else if (lightSource.type == LIGHT_AREA ) {
        float3 localOrigin = (float3)(lightSource.size*(-0.5f+uv), 0.f);

        *origin = transformPoint(lightSource.tm, localOrigin);
        // Area light
        float3 pointOnPlane = bbox.pMin+ (bbox.pMax-bbox.pMin)*(float3)(rndNum.x, rndNum.y, rndNum.z);
        *wi = normalize(pointOnPlane-*origin);
        *pdf = lightSource.area; 

        *power = lightSource.radiance / *pdf;
    } 
    else if (lightSource.type == LIGHT_DIRECTIONAL) {

            float3 localOrigin = (float3)(lightSource.size*(-0.5f+uv), 0.f); 
            
            *origin = transformPoint(lightSource.tm, localOrigin);
            
            // Directional light
            float3 pointOnPlane = transformPoint(lightSource.tm, (float3)(localOrigin.xy, 0.1f));
            *wi = normalize(pointOnPlane-*origin);
            *pdf = lightSource.area; 
            *power = lightSource.radiance;

    } else { // if (lightSource.type == LIGHT_CONE ) 
        float3 localOrigin = (float3)(0.0f, 0.f, 0.f);
        *origin = transformPoint(lightSource.tm, localOrigin);

        float3 localDir = uniformSampleCone(uv, lightSource.cosFOV);
        float3 pointOnPlane = transformPoint(lightSource.tm, localDir);
        *wi = normalize(pointOnPlane-*origin);
        *pdf = uniformConePdf(lightSource.cosFOV); 
        
        *power = localDir.z*localDir.z*localDir.z*localDir.z*lightSource.radiance*localDir.z/(*pdf);
    }  

 
}

  

#endif // LIGHT_CL
