#include <inviwo/core/properties/simplelightingproperty.h>

namespace inviwo {

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
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)
    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(1.0f, 0.0f, 0.0f)) {

    shadingMode_.addOption("none", "No Shading");
    shadingMode_.addOption("ambient", "Ambient");
    shadingMode_.addOption("diffuse", "Diffuse");
    shadingMode_.addOption("specular", "Specular");
    shadingMode_.addOption("phong", "Phong");
    shadingMode_.setSelectedIdentifier("phong");
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
