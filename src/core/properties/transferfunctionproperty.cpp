#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

    TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics )
        : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
    {}

    void TransferFunctionProperty::serialize(IvwSerializer& s) const {
        //Property::serialize(s) ;
        //s.serialize("value", get());
    }

    void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
        //Property::deserialize(d) ;
        //TransferFunction value;
        //d.deserialize("value", value);
        //set(value);
    }
} // namespace
