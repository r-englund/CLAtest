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

#include "directionallightsourceprocessor.h"

namespace inviwo {

ProcessorClassName(DirectionalLightSourceProcessor, "Directional light source"); 
ProcessorCategory(DirectionalLightSourceProcessor, "Light source");
ProcessorCodeState(DirectionalLightSourceProcessor, CODE_STATE_EXPERIMENTAL);


DirectionalLightSourceProcessor::DirectionalLightSourceProcessor()
    : Processor(),
      outport_("DirectionalLightSource"),
      lightPowerProp_("lightPower", "Light power (%)", 50.f, 0.f, 100.f),
      lightDiffuse_("lightDiffuse", "Color", vec4(1.0f)),
      lightPosition_("lightPosition", "Light Source Position", vec3(1.f, 0.65f, 0.65f), vec3(-1.f), vec3(1.f)),
      lightSize_("lightSize", "Light size", vec2(1.5f, 1.5f), vec2(0.0f, 0.0f), vec2(3.0f, 3.0f))
{
    addPort(outport_);

    addProperty(lightPosition_);
    addProperty(lightDiffuse_);
    addProperty(lightPowerProp_);
    addProperty(lightSize_);

    // assign lighting properties to property group
    lightPosition_.setGroupID("lighting");
    lightDiffuse_.setGroupID("lighting");
    lightPowerProp_.setGroupID("lighting");
    lightSize_.setGroupID("lighting");

    Property::setGroupDisplayName("lighting", "Light Parameters");

    lightPosition_.setSemantics(PropertySemantics::LightPosition);
    lightDiffuse_.setSemantics(PropertySemantics::Color);

    lightSource_ = new DirectionalLight();
}

DirectionalLightSourceProcessor::~DirectionalLightSourceProcessor() {
    delete lightSource_;
};

void DirectionalLightSourceProcessor::process() {
   updateDirectionalLightSource(lightSource_);
   outport_.setData(lightSource_, false);

}

void DirectionalLightSourceProcessor::updateDirectionalLightSource(DirectionalLight* lightSource) {
    vec3 lightPos = vec3(0.5f, 0.5f, 0.5f) + lightPosition_.get()*10.f/*+vec3(20.f, 20.f, 20.f)*/;
    vec3 dir = glm::normalize(vec3(0.5f, 0.5f, 0.5f)-lightPos);

    vec3 A = vec3(0,0,1);
    vec3 B = dir;//B(0,1,0);
    float angle = acos(glm::dot(A,B));
    vec3 rotationAxis = glm::normalize(glm::cross(A, B));

#ifndef GLM_FORCE_RADIANS
    angle = glm::degrees(angle);
#endif // GLM_FORCE_RADIANS

    mat4 transformationMatrix = glm::translate(lightPos)*glm::rotate(angle, rotationAxis);

    lightSource->setObjectToTexture(transformationMatrix);
    lightSource->setSize(lightSize_.get());
    vec3 diffuseLight = lightDiffuse_.get().xyz();
    lightSource->setIntensity(lightPowerProp_.get()*diffuseLight);
    lightSource->setLightDirection(dir);
}

} // namespace
