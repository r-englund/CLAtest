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

#include <inviwo/core/datastructures/baselightsource.h>



namespace inviwo {

PackedLightSource baseLightToPackedLight(const LightSource* lightsource, float radianceScale)
{
    PackedLightSource light;
    light.tm = lightsource->getObjectToTexture();
    light.radiance = vec4(radianceScale*lightsource->getPower(), 1.f);
    light.type = lightsource->getLightSourceType();
    light.area = lightsource->getArea();
    light.cosFOV = std::cos( glm::radians(lightsource->getFieldOfView()/2.f) );
    light.size = lightsource->getSize();

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
