#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {

    FileProperty::FileProperty(std::string identifier, std::string displayName,std::string value, PropertyOwner::InvalidationLevel invalidationLevel, 
PropertySemantics::Type semantics)
    : TemplateProperty<std::string>(identifier, displayName,value, invalidationLevel, semantics)
{}

int FileProperty::getVariantType() {
    return Variant::VariantTypeString;
}

Variant FileProperty::getVariant() {
    return Variant(get());
}

void  FileProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getString());
    }
}


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
