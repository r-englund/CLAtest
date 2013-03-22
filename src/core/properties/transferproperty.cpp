#include <inviwo/core/properties/transferproperty.h>

namespace inviwo {

    TransferProperty::TransferProperty(std::string identifier, std::string displayName, TransferFunc value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
        : TemplateProperty<TransferFunc>(identifier, displayName,value, semantics)
    {}

    void TransferProperty::serialize(IvwSerializer& s) const {
        //Property::serialize(s) ;
        //s.serialize("value", get());
    }

    void TransferProperty::deserialize(IvwDeserializer& d) {
        //Property::deserialize(d) ;
        //TransferFunc value;
        //d.deserialize("value", value);
        //set(value);
    }
} // namespace
