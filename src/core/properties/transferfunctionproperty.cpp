#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics )
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
{}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {}

} // namespace
