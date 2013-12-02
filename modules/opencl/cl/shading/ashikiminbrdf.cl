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

#ifndef ASHIKIMIM_BRDF_CL
#define ASHIKIMIM_BRDF_CL

#include "shading/shadingmath.cl"
#include "shading/microfacet.cl"

float AshikimBRDF(const float3 wo, const float3 wi, const float3 N, const float f0, const float exponent) {
    //float cosThetaWo = fabs(dot(wo, N));
    //float cosThetaWi = fabs(dot(wi, N));
    float cosThetaWo = absCosTheta(wo);
    float cosThetaWi = absCosTheta(wi);

    //if ( cosThetaWo*cosThetaWi < 1e-4f ) 
    //    return (float)(0.f);

    if ( cosThetaWo == 0.f || cosThetaWi == 0.f ) 
        return (float)(0.f);

    float3 wh = wo+wi;
    //if ( all(wh < 1e-5f) ) 
    //    return (float)(0.f);

    wh = fast_normalize(wh);

    float NdotWh = absCosTheta(wh);
    // Fresnel term
    //float f0 = pown((1-etai)/(1+etat), 2);
    //float f0 = pown((1.f-n)/(1.f+n), 2);
    float F = fresnelSchlick(NdotWh, f0); 
    //float F = fresnelDielectric(etai, etat, NdotWh);
    //float NdotWh = fabs(dot(wh, N));
    float WodotWh = fabs(dot(wh, wo));
    // Distribution term
    float D = BlinnPhongDistribution(NdotWh, exponent);
    // Geometry term
    float G = CookTorranceGeometry(wo, wi, wh); //minf(1.f, minf( 2.f*NdotWh*cosThetaWo/WodotWh, 2.f*NdotWh*cosThetaWi/WodotWh ) );
    // Add a lambertian factor (1/pi)
    //return D * G * F / (4.0f * cosThetaWi * cosThetaWo);
    //return D * G * F / (4.0f * cosThetaWi * cosThetaWo);
    float denom = max(1e-4f, 4.0f * cosThetaWi * cosThetaWo);
    return native_divide(D * G * F , denom);
    
}

float3 AshikhminSample(const float3 wi, const float3 N, float exponent, const float2 rnd) {
    return BlinnSample(wi, N, exponent, rnd);
}



float AshikhminPdf(const float3 wo, const float3 wi, const float exponent) {
    return BlinnPdf(wo, wi, exponent);  
}


#endif
