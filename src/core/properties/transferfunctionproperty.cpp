#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

    TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
        : TemplateProperty<TransferFunction>(identifier, displayName,value, semantics)
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
