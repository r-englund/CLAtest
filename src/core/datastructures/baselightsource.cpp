#include <inviwo/core/datastructures/baselightsource.h>



namespace inviwo {

PackedLightSource baseLightToPackedLight(const BaseLightSource* lightsource, float radianceScale)
{
    PackedLightSource light;
    light.tm = lightsource->tm;
    light.radiance = radianceScale*lightsource->radiance;
    light.type = lightsource->type;
    light.area = lightsource->area;
    light.cosFOV = std::cos( glm::radians(lightsource->fov/2.f) );
    light.size = lightsource->size;

    return light;
}

uvec2 getSamplesPerLight(uvec2 nSamples, int nLightSources) 
{
    uvec2 samplesPerLight;

    //samplesPerLight.y = nPhotons.y / nLightSources;
    //samplesPerLight.x = (int)sqrt((float)nPhotons.x*samplesPerLight.y);

    unsigned int nPhotons = nSamples.x*nSamples.y;
    samplesPerLight.y = nPhotons / nLightSources;
    samplesPerLight.x = (unsigned int)sqrt((float)samplesPerLight.y);
    samplesPerLight.y = samplesPerLight.x*samplesPerLight.x;

    return samplesPerLight;
}

}
