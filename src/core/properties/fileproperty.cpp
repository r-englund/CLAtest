#include "inviwo/core/properties/fileproperty.h"

namespace inviwo {

FileProperty::FileProperty(std::string identifier, std::string displayName, std::string value)
    : TemplateProperty(identifier, displayName, value)
{}

void FileProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;
    s.serialize("url", get());
}

void FileProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string value;
    d.deserialize("url", value);
    set(value);
}

} // namespace
