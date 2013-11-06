#ifndef SHADING_CL
#define SHADING_CL

#include "shading/phasefunctions.cl" 
#include "shading/blinnphongbrdf.cl"
#include "shading/cooktorrancebrdf.cl"
#include "shading/ashikiminbrdf.cl"
#include "shading/wardbrdf.cl"
#include "shading/abcbrdf.cl"

#include "random.cl"
#include "gradients.cl"
#include "samplers.cl"  
// Take a look at this pdf, it explains many of the models used:
// http://www.cs.ucl.ac.uk/staff/j.kautz/GameCourse/04_PointLights.pdf

#define PHASE_FUNCTION_ISOTROPIC 1
#define PHASE_FUNCTION_HENYEY_GREENSTEIN 2
#define PHASE_FUNCTION_SCHLICK 3
#define PHASE_FUNCTION_BLINN_PHONG 4
#define PHASE_FUNCTION_WARD 5
#define PHASE_FUNCTION_COOK_TORRANCE 6
#define PHASE_FUNCTION_ABC_MICROFACET 7
#define PHASE_FUNCTION_ASHIKHMIN 8
#define PHASE_FUNCTION_MIX 9
#define PHASE_FUNCTION_HALF_ANGLE_SLICING 10
#define PHASE_FUNCTION_SUB_SURFACE_SCATTERING 11


typedef enum ShadingType {
    ISOTROPIC = 1,
    HENYEY_GREENSTEIN,
    SCHLICK,
    BLINN_PHONG,
    WARD,
    COOK_TORRANCE,
    ABC_MICROFACET,
    ASHIKHMIN,
    MIX,
    HYBRID,
    HALF_ANGLE_SLICING,
    SUB_SURFACE_SCATTERING
} ShadingType;

#define BLINN_EXPONENT 10.0f
#define GRADIENT_MAGNITUDE_THRESHOLD (MAX_GRAD_MAG*0.5f)
    // Material properties: 
    // x = fresnel factor f_0 = ((1-n)/(1+n))^2, 
    // y = exponent (blinn)
    // z = phase function scattering
#define EXPONENT_SCALE 100.f



// Maps parameter x from 0-1. Used for exposure in Blinn-Phong and other parameters. 
inline float vrayParameterMapping(float x) {
    return 1.f/native_powr(1.f - x, 3.5f) - 1.f;
}
inline float WardParameterMapping(float x) {
    return fmax(x,1e-4f);
    //return native_powr(x, 2.f);
}
// Convert between the roughness m in Beckmann distribution and shininess exponent in Blinn-Phong distribution
float shininessToRoughness(float exponent) {
    return native_sqrt( 2.f/(exponent+2.f) );
}
float roughnessToShininess(float m) {
    return 2.f/(fmax(1e-4f, m*m))-2.f;
}
float2 BCParameterMapping(float2 bc) {
    return bc*(float2)(13000.f,1.f); 
}
float HenyehGreensteinToSchlick(float g) {
    // Should work for intermediate values of k in Schlick 
    return 1.55f*g-0.55f*pown(g, 3); 
    //return 1.5f*g-(1.f-1.5f)*g*g*g; 
}

inline float4 scaleShadingParameter(float4 material, const ShadingType shadingType) {
    float4 scaledMaterial = material;
#ifdef PERFORMANCE 
#if PHASE_FUNCTION == PHASE_FUNCTION_HENYEY_GREENSTEIN
    scaledMaterial.z = -1.f+2.f*material.z;  
#elif PHASE_FUNCTION == PHASE_FUNCTION_SCHLICK
    scaledMaterial.z = HenyehGreensteinToSchlick(-1.f+2.f*material.z); 
#elif PHASE_FUNCTION == PHASE_FUNCTION_BLINN_PHONG
    scaledMaterial.y = roughnessToShininess(material.y);  
#elif PHASE_FUNCTION == PHASE_FUNCTION_WARD
    scaledMaterial.y = WardParameterMapping(material.y);  
#elif PHASE_FUNCTION == PHASE_FUNCTION_COOK_TORRANCE
    //scaledMaterial.y = roughnessToShininess(material.y);  
    scaledMaterial.y = WardParameterMapping(material.y); 
#elif PHASE_FUNCTION == PHASE_FUNCTION_ABC_MICROFACET
    scaledMaterial.yz = BCParameterMapping(material.yz);  
#elif PHASE_FUNCTION == PHASE_FUNCTION_ASHIKHMIN 
    scaledMaterial.y = roughnessToShininess(material.y);  
#elif PHASE_FUNCTION == PHASE_FUNCTION_MIX
    scaledMaterial.y = roughnessToShininess(material.y);  
#else  
    scaledMaterial.y = material.y; 
#endif
#else
    if ( shadingType == HENYEY_GREENSTEIN ) {
        scaledMaterial.z = -1.f+2.f*material.z;  
    }else if ( shadingType == SCHLICK ) {
        scaledMaterial.z = HenyehGreensteinToSchlick(-1.f+2.f*material.z); 
    }else if ( shadingType == BLINN_PHONG ) {
        scaledMaterial.y = roughnessToShininess(material.y);  
    } else if ( shadingType == WARD ) {
        scaledMaterial.y = WardParameterMapping(material.y);  
    } else if ( shadingType == COOK_TORRANCE ) {
        //scaledMaterial.y = roughnessToShininess(material.y);  
        scaledMaterial.y = WardParameterMapping(material.y); 
    } else if ( shadingType == ABC_MICROFACET ) {
        scaledMaterial.yz = BCParameterMapping(material.yz);  
    } else if ( shadingType == ASHIKHMIN ) {
        scaledMaterial.y = roughnessToShininess(material.y);  
    } else if ( shadingType == MIX ) {
        scaledMaterial.y = roughnessToShininess(material.y);  
    } else {
        scaledMaterial.y = material.y;  
    }
#endif
    // Scale index of refraction (IOR) f0 = ((n1-n2)/(n1+n2))^2, where n1 = 1
    // n2=20 gives f0=0.8185
    scaledMaterial.x *= 0.81859410430839002267573696145125f;
    //scaledMaterial.x *= 4.f;
    return scaledMaterial;
}

void sampleShadingFunction(read_only image3d_t volumeTex, float volumeSample, const float3 invSpacing, read_only image2d_t tfMaterial,
                 float3 sample, 
                 float3* direction, random_state* randstate, const ShadingType shadingType) {

    // Material properties: 
    // x = fresnel factor f_0 = ((1-n)/(1+n))^2, 
    // y = exponent (blinn)
    // z = phase function scattering
    float4 material = read_imagef(tfMaterial, smpNormClampEdgeLinear, (float2)(volumeSample,0.5f));
    // float exponent = 2.f/(material.y*material.y)-2.f;
    //float exponent =material.y;//*100.f;
    // Map parameter according to V-ray
    material = scaleShadingParameter(material, shadingType); 
    float2 rnd = (float2)(random_01(randstate), random_01(randstate));
    if ( shadingType == HENYEY_GREENSTEIN ) {
        *direction = HenyeyGreensteinSample(-*direction, material.z, rnd);            
    } else if ( shadingType == SCHLICK ) { 
         *direction = SchlickSample(-*direction, material.z, rnd); 
    } else if ( shadingType == BLINN_PHONG ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv, gradient, BlinnSample(wo, gradient, material.y, rnd));
    } else if ( shadingType == WARD ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv, gradient, WardSample(wo, gradient, material.y,material.y, rnd));
    } else if ( shadingType == COOK_TORRANCE ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv, gradient, CookTorranceSample(wo, material.y, rnd));
    } else if ( shadingType == ABC_MICROFACET ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv, gradient, ABCMicrofacetSample(wo, material.z, material.w, rnd));
    } else if ( shadingType == HALF_ANGLE_SLICING ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv,gradient, uniformSampleCone(rnd, 0.5f));
    } else if ( shadingType == ASHIKHMIN ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(sample), volumeTex).xyz);
        float3 wo = -*direction;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        *direction = shadingToWorld(Nu, Nv, gradient, AshikhminSample(wo, gradient, material.y, rnd));
    } else if ( shadingType == MIX ) {
        float3 gradient = invSpacing*(calcGradient(as_float4(sample), volumeTex).xyz); 
        //float gradientMagnitude = length(gradient);
        //if(gradientMagnitude > GRADIENT_MAGNITUDE_THRESHOLD) {
        
        //if(material.w > 0.5f) {    
        if(random_01(randstate) >= material.w) {
            *direction = uniformSampleSphere(rnd);   
        } else {
            gradient = normalize(gradient);
            float3 wo = -*direction;
            float3 Nu = normalize(cross(gradient, wo)); 
            float3 Nv = normalize(cross(gradient, Nu));
            wo = worldToShading(Nu, Nv, gradient, wo); 
            *direction = shadingToWorld(Nu, Nv, gradient, AshikhminSample(wo, gradient, material.y, rnd)); 
        }
    } else { 
        *direction = uniformSampleSphere(rnd);  
    }

}

void sampleShadingFunctionPdf(read_only image3d_t volumeTex, float volumeSample, const float3 invSpacing, float4 material,
                 float3 pos, float3 toCameraDir,
                 float3* toLightDirection, float *pdf, random_state* randstate, const ShadingType shadingType) {

    float2 rnd = (float2)(random_01(randstate), random_01(randstate));
    if ( shadingType == HENYEY_GREENSTEIN ) {
        *toLightDirection = HenyeyGreensteinSample(-toCameraDir, material.z, rnd);     
        *pdf = HenyeyGreensteinPhaseFunction(-toCameraDir, *toLightDirection, material.z);    
    } else if ( shadingType == SCHLICK ) { 
         *toLightDirection = SchlickSample(-toCameraDir, material.z, rnd); 
          *pdf = SchlickPhaseFunction(-toCameraDir, *toLightDirection, material.z); 
    } else if ( shadingType == BLINN_PHONG ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo);
        float3 wi = BlinnSample(wo, gradient, material.y, rnd);
        *pdf = BlinnPdf(wo, wi, material.y);
        *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
    } else if ( shadingType == WARD ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo);
        float3 wi = WardSample(wo, gradient, material.y,material.y, rnd);
        *pdf = WardPdf(wo, wi, gradient, material.y, material.y);
        *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
    } else if ( shadingType == COOK_TORRANCE ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = CookTorranceSample(wo, material.y, rnd);
        *pdf = CookTorrancePdf(wo, wi, gradient, material.y);
        *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
    } else if ( shadingType == ABC_MICROFACET ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = ABCMicrofacetSample(wo, material.z, material.w, rnd);
        // TODO: Implement ABC pdf
        *pdf = CookTorrancePdf(wo, wi, gradient, material.w);
        *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
    } else if ( shadingType == HALF_ANGLE_SLICING ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = uniformSampleCone(rnd, 0.5f);
        *pdf = uniformConePdf(0.5f);
        *toLightDirection = shadingToWorld(Nu, Nv,gradient, wi);
    } else if ( shadingType == ASHIKHMIN ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = AshikhminSample(wo, gradient, material.y, rnd);
        *pdf = AshikhminPdf(wo, wi, material.y);
        *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
    } else if ( shadingType == MIX ) {
        float3 gradient = invSpacing*(calcGradient(as_float4(pos), volumeTex).xyz); 
        //float gradientMagnitude = length(gradient);
        //if(gradientMagnitude > GRADIENT_MAGNITUDE_THRESHOLD) {
        
        //if(material.w > 0.5f) {    
        if(random_01(randstate) >= material.w) {

            *toLightDirection = uniformSampleSphere(rnd);   
            *pdf = uniformSpherePdf();
        } else {
            gradient = normalize(gradient);
            float3 wo = toCameraDir;
            float3 Nu = normalize(cross(gradient, wo)); 
            float3 Nv = normalize(cross(gradient, Nu));
            wo = worldToShading(Nu, Nv, gradient, wo); 
            float3 wi = AshikhminSample(wo, gradient, material.y, rnd);
            *pdf = AshikhminPdf(wo, wi, material.y);
            *toLightDirection = shadingToWorld(Nu, Nv, gradient, wi);
        }
    } else { 
        *toLightDirection = normalize(uniformSampleSphere(rnd));  
        *pdf = uniformSpherePdf();
    }

}
float shadingFunctionPdf(read_only image3d_t volumeTex, float volumeSample, const float3 invSpacing, float4 material,
                 float3 pos, float3 toCameraDir,
                 float3 toLightDirection, const ShadingType shadingType) {

    float pdf;
    if ( shadingType == HENYEY_GREENSTEIN ) {
        pdf = HenyeyGreensteinPhaseFunction(-toCameraDir, toLightDirection, material.z);    
    } else if ( shadingType == SCHLICK ) { 
        pdf = SchlickPhaseFunction(-toCameraDir, toLightDirection, material.z); 
    } else if ( shadingType == BLINN_PHONG ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo);
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        pdf = BlinnPdf(wo, wi, material.y);
    } else if ( shadingType == WARD ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo);
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        pdf = WardPdf(wo, wi, gradient, material.y, material.y);
    } else if ( shadingType == COOK_TORRANCE ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        pdf = CookTorrancePdf(wo, wi, gradient, material.y);
    } else if ( shadingType == ABC_MICROFACET ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        // TODO: Implement ABC pdf
        pdf = CookTorrancePdf(wo, wi, gradient, material.w);
    } else if ( shadingType == HALF_ANGLE_SLICING ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        pdf = uniformConePdf(0.5f);
    } else if ( shadingType == ASHIKHMIN ) {
        float3 gradient = normalize(invSpacing*calcGradient(as_float4(pos), volumeTex).xyz);
        float3 wo = toCameraDir;
        float3 Nu = normalize(cross(gradient, wo));
        float3 Nv = normalize(cross(gradient, Nu));
        wo = worldToShading(Nu, Nv, gradient, wo); 
        float3 wi = worldToShading(Nu, Nv, gradient, toLightDirection);
        pdf = AshikhminPdf(wo, wi, material.y);
    } else if ( shadingType == MIX ) {  
        pdf = uniformSpherePdf();
    } else { 
        pdf = uniformSpherePdf();
    }
    return pdf;
}
#endif
