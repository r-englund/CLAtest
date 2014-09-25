#ifndef IVW_SIMPLELIGHTINGPROPERTY_H
#define IVW_SIMPLELIGHTINGPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

/** \class SimpleLightingProperty
 * Add simple light properties, position, ambient, diffuse, specular color. Used for phong shading.
 */

namespace ShadingMode {
    enum Modes {
        None,
        Ambient,
        Diffuse,
        Specular,
        Phong,
    };
}

class IVW_CORE_API SimpleLightingProperty : public CompositeProperty {
public:
    SimpleLightingProperty(std::string identifier, std::string displayName,
                           PropertyOwner::InvalidationLevel = PropertyOwner::INVALID_OUTPUT,
                           PropertySemantics semantics = PropertySemantics::Default);
    virtual ~SimpleLightingProperty(){}

    virtual std::string getClassIdentifier()  const { return "SimpleLightingProperty"; }

    OptionPropertyInt shadingMode_;
    FloatVec3Property lightPosition_;
    FloatVec3Property lightColorAmbient_;
    FloatVec3Property lightColorDiffuse_;
    FloatVec3Property lightColorSpecular_;
    IntProperty lightSpecularExponent_;
    BoolProperty applyLightAttenuation_;
    FloatVec3Property lightAttenuation_;

};

} // namespace

#endif // IVW_SIMPLELIGHTINGPROPERTY_H

