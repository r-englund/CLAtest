#ifndef LIGHT_CL
#define LIGHT_CL

#include "datastructures/lightsource.cl"
#include "random.cl"
#include "shading/shadingmath.cl"
#include "transformations.cl"



int getLightSourceId(const int2 nPhotonsPerLight, const int nLightSources) {
    //return get_global_id(1)/nPhotonsPerLight.y;
    return (get_global_id(1)*get_global_size(0)+get_global_id(0))/nPhotonsPerLight.y;
    //return min(nLightSources-1, globalId/(nPhotonsPerLight.x*nPhotonsPerLight.y));
    //int globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    //return min(nLightSources-1, globalId/(nPhotonsPerLight.x*nPhotonsPerLight.y));
}



float2 getLightSourceUniformUV(const int2 nPhotonsPerLight, const int nLightSources) {
    int globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    //int maxPhotons = get_global_size(1)*get_global_size(0);
    int lightSourceId =  getLightSourceId(nPhotonsPerLight, nLightSources);
 
    int lightSourceOffset = lightSourceId*nPhotonsPerLight.y;


    int lightSampleIndex = globalId-lightSourceOffset;
    int2 photonId = (int2)(lightSampleIndex%nPhotonsPerLight.x, lightSampleIndex/nPhotonsPerLight.x);

    return (float2)((convert_float2(photonId)+0.5f)/convert_float(nPhotonsPerLight.x));
}
float2 getLightSourceStratifiedUV(const int2 nPhotonsPerLight, const int nLightSources, const float2 randNum) {
    int globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    //int maxPhotons = get_global_size(1)*get_global_size(0);
    int lightSourceId =  getLightSourceId(nPhotonsPerLight, nLightSources);

    int lightSourceOffset = lightSourceId*nPhotonsPerLight.y;


    int lightSampleIndex = globalId-lightSourceOffset;
    // Calculate integer xy coordinate on light
    int2 photonId = (int2)(lightSampleIndex%nPhotonsPerLight.x, lightSampleIndex/nPhotonsPerLight.x);

    return (float2)((convert_float2(photonId)+randNum)/convert_float(nPhotonsPerLight.x));
}
  
void sampleLights(__global PhotonLightSource const * __restrict lightSources, const int2 nPhotonsPerLight,
                  const int nLightSources, const float2 uv, float3* __restrict origin, float3* __restrict wi, float3* __restrict power, float* __restrict pdf, random_state* __restrict randstate, const BBox bbox) {
 
    uint globalId = get_global_id(1)*get_global_size(0)+get_global_id(0);
    int lightSourceId = getLightSourceId(nPhotonsPerLight, nLightSources);



    PhotonLightSource lightSource = lightSources[lightSourceId];
    //if(sizeof(PhotonLightSource) != 128)
    //    lightSource.radiance = (float3)(0);
    //if( lightSourceId == 0) {
    //    lightSource = lightSources[lightSourceId];
    //} else {
    //    lightSource.type = LIGHT_POINT;
    //    //lightSource.radiance = 1000000000.f;
    //}
    //float2 uv = (float2)(get_global_id(0)+0.5f, get_global_id(1)+0.5f)/(float2)(get_global_size(0),get_global_size(1));
    if( lightSource.type == LIGHT_POINT ) {
        float3 localOrigin = (float3)(0.f);
        *origin = transformPoint(lightSource.tm, localOrigin);
        //float3 sphereSample = sphericalToCartesian(1.f, uv.x*2.f*M_PI, uv.y*M_PI);
        float3 sphereSample = uniformSampleSphere(uv);
        //*wi = -(float3)(0.f, 0.f, 1.f);
        //*origin = (float3)(0.5f, 0.5f, 2.f);
        *wi = -sphereSample;

        *pdf = uniformSpherePdf();
        *power = lightSource.radiance / *pdf;
    } 
    else if (lightSource.type == LIGHT_AREA ) {
        float3 localOrigin = (float3)(lightSource.size*(-0.5f+uv), 0.f);

        *origin = transformPoint(lightSource.tm, localOrigin);
        // Area light
        float3 pointOnPlane = bbox.pMin+ (bbox.pMax-bbox.pMin)*(float3)(random_01(randstate), random_01(randstate), random_01(randstate));
        *wi = normalize(pointOnPlane-*origin);
        *pdf = lightSource.area; 

        *power = lightSource.radiance / *pdf;
    } 
    else if (lightSource.type == LIGHT_DIRECTIONAL || lightSource.type == LIGHT_AREA ) {

            float3 localOrigin = (float3)(lightSource.size*(-0.5f+uv), 0.f); 
            
            //*origin = translatePoint(lightSource.tm, localOrigin);
            *origin = transformPoint(lightSource.tm, localOrigin);
            // Area light
            //float3 pointOnPlane = bbox.pMin+ (bbox.pMax-bbox.pMin)*(float3)(random_01(randstate), random_01(randstate), random_01(randstate));
            // Directional light
            float3 pointOnPlane = transformPoint(lightSource.tm, (float3)(localOrigin.xy, 0.1f));
            *wi = normalize(pointOnPlane-*origin);
            *pdf = lightSource.area; 
            *power = lightSource.radiance;
            // Fixed plane 
            //*origin = (float3)(uv.x, -1.0f+uv.y,  1.5f);
            //*wi = normalize((float3)(0.0f, 0.f, -1.f));
            //*power = 0.01f*lightSource.radiance;
            //*pdf = 1.f;

    } else { // if (lightSource.type == LIGHT_CONE ) 
        float3 localOrigin = (float3)(0.0f, 0.f, 0.f);
        *origin = transformPoint(lightSource.tm, localOrigin);
        //float3 pointOnPlane = transformPoint(lightSource.tm, (float3)(-0.5f+uv.x, -0.5f+uv.y, 0.f));
        float3 localDir = uniformSampleCone(uv, lightSource.cosFOV);
        float3 pointOnPlane = transformPoint(lightSource.tm, localDir);
        *wi = normalize(pointOnPlane-*origin);
        *pdf = uniformConePdf(lightSource.cosFOV); 
        
        //float3 sphereSample = sphericalToCartesian(1.f, uv.x*2.f*M_PI, uv.y*M_PI);
        //float3 sphereSample = normalize(uniformSampleSphere(random_01(randstate), random_01(randstate)));
        //*wi = -sphereSample;
        //*origin = (float3)(0.5f)+2.f*sphereSample;
        //*wi = -normalize((float3)(0.f, 1.f, 1.f));
        //*origin = (float3)(0.5f)-(*wi)*3.f;
        *power = localDir.z*localDir.z*localDir.z*localDir.z*lightSource.radiance*localDir.z/(*pdf);//localDir.z*localDir.z*localDir.z;//fabs(dot(localOrigin, localDir));
        //*power = lightSource.radiance / *pdf;//localDir.z*localDir.z*localDir.z;//fabs(dot(localOrigin, localDir));
        //*power = lightSource.radiance*localDir.z/(*pdf);//localDir.z*localDir.z*localDir.z;//fabs(dot(localOrigin, localDir));
        


    }  

 
}

  

#endif // LIGHT_CL
