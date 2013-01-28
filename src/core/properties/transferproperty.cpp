#include <inviwo/core/properties/transferproperty.h>

namespace inviwo {

TransferProperty::TransferProperty(std::string identifier, std::string displayName, bool value)
    : TemplateProperty(identifier, displayName, value)
{}

void TransferProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("value", get());
}

void TransferProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    bool value;
    d.deserialize("value", value);
    set(value);
}

} // namespace
