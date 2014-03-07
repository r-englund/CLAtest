/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "spotlightsourceprocessor.h"

namespace inviwo {

ProcessorClassName(SpotLightSourceProcessor, "Spot light source");
ProcessorCategory(SpotLightSourceProcessor, "Light source");
ProcessorCodeState(SpotLightSourceProcessor, CODE_STATE_EXPERIMENTAL);

SpotLightSourceProcessor::SpotLightSourceProcessor()
    : Processor()
    , outport_("SpotLightSource")
    , lightPowerProp_("lightPower", "Light power (%)", 50.f, 0.f, 100.f)
    , lightSize_("lightSize", "Light size", vec2(1.5f, 1.5f), vec2(0.0f, 0.0f), vec2(3.0f, 3.0f))
    , lightDiffuse_("lightDiffuse", "Color", vec4(1.0f))
    , lightPosition_("lightPosition", "Light Source Position", vec3(1.f, 0.65f, 0.65f), vec3(-1.f), vec3(1.f)) 
    , lightCuttOffAngle_("lightCuttOffAngle", "Light Cut Off Angle", 30.f, 5.f, 90.f) {
    addPort(outport_);
    addProperty(lightPosition_);
    addProperty(lightCuttOffAngle_);
    addProperty(lightDiffuse_);
    addProperty(lightPowerProp_);
    addProperty(lightSize_);
    // assign lighting properties to property group
    lightPosition_.setGroupID("lighting");
    lightCuttOffAngle_.setGroupID("lighting");
    lightDiffuse_.setGroupID("lighting");
    lightPowerProp_.setGroupID("lighting");
    lightSize_.setGroupID("lighting");
    Property::setGroupDisplayName("lighting", "Light Parameters");
    lightPosition_.setSemantics(PropertySemantics::LightPosition);
    lightDiffuse_.setSemantics(PropertySemantics::Color);
    lightSource_ = new SpotLight();
}

SpotLightSourceProcessor::~SpotLightSourceProcessor() {
    delete lightSource_;
}

void SpotLightSourceProcessor::process() {
    updateSpotLightSource(lightSource_);
    outport_.setData(lightSource_, false);
}

void SpotLightSourceProcessor::updateSpotLightSource(SpotLight* lightSource) {
    vec3 lightPos = vec3(0.5f, 0.5f, 0.5f) + lightPosition_.get();
    vec3 dir = glm::normalize(vec3(0.5f, 0.5f, 0.5f)-lightPos);
    mat4 transformationMatrix = getLightTransformationMatrix(lightPos, dir);
    lightSource->setObjectToTexture(transformationMatrix);
    lightSource->setSize(lightSize_.get());
    vec3 diffuseLight = lightDiffuse_.get().xyz();
    lightSource->setIntensity(lightPowerProp_.get()*diffuseLight);
    lightSource->setPosition(lightPosition_.get());
    lightSource->setDirection(dir);
    lightSource->setCutOffAngle(lightCuttOffAngle_.get());
    lightSource->setCutOffFactor(glm::cos<float>(glm::radians(lightCuttOffAngle_.get())));
}

} // namespace
