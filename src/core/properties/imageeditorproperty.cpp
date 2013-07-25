#include <inviwo/core/properties/imageeditorproperty.h>

namespace inviwo {

    ImageEditorProperty::ImageEditorProperty(std::string identifier, std::string displayName,std::string value, PropertyOwner::InvalidationLevel invalidationLevel, 
PropertySemantics::Type semantics)
    : FileProperty(identifier, displayName,value, invalidationLevel, semantics)
{}


} // namespace
