/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/properties/simplelightingproperty.h>

namespace inviwo {

PropertyClassIdentifier(SimpleLightingProperty, "org.inviwo.SimpleLightingProperty");

SimpleLightingProperty::SimpleLightingProperty(std::string identifier, std::string displayName,
                                               PropertyOwner::InvalidationLevel invalidationLevel,
                                               PropertySemantics semantics)
    : CompositeProperty(identifier, displayName, invalidationLevel, semantics)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , lightPosition_("lightPosition", "Position", vec3(0.0f, 0.7071f, 0.7071f), vec3(-10, -10, -10),
                     vec3(10, 10, 10))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(0.1f, 0.1f, 0.1f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.75f, 0.75f, 0.75f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(0.75f, 0.75f, 0.75f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 1, 180)
    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(1.0f, 0.0f, 0.0f)) {

    shadingMode_.addOption("none", "No Shading", ShadingMode::None);
    shadingMode_.addOption("ambient", "Ambient", ShadingMode::Ambient);
    shadingMode_.addOption("diffuse", "Diffuse", ShadingMode::Diffuse);
    shadingMode_.addOption("specular", "Specular", ShadingMode::Specular);
    shadingMode_.addOption("phong", "Phong", ShadingMode::Phong);
    shadingMode_.setSelectedValue(ShadingMode::Phong);
    shadingMode_.setCurrentStateAsDefault();

    lightPosition_.setSemantics(PropertySemantics("Spherical"));
    lightColorAmbient_.setSemantics(PropertySemantics::Color);
    lightColorDiffuse_.setSemantics(PropertySemantics::Color);
    lightColorSpecular_.setSemantics(PropertySemantics::Color);

    // add properties
    addProperty(shadingMode_);
    addProperty(lightPosition_);
    addProperty(lightColorAmbient_);
    addProperty(lightColorDiffuse_);
    addProperty(lightColorSpecular_);
    addProperty(lightSpecularExponent_);
    addProperty(applyLightAttenuation_);
    addProperty(lightAttenuation_);
}

}  // namespace
