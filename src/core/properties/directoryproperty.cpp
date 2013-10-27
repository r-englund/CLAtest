#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/util/filedirectory.h>

namespace inviwo {

DirectoryProperty::DirectoryProperty(std::string identifier, std::string displayName,std::string value, PropertyOwner::InvalidationLevel invalidationLevel, 
    PropertySemantics::Type semantics)
    : TemplateProperty<std::string>(identifier, displayName,value, invalidationLevel, semantics)
{}


std::vector<std::string> DirectoryProperty::getDirectoryTree()  const  {    
    return directoryTree_;
}

void DirectoryProperty::setDirectoryTree(std::vector<std::string> dirTree)  {    
    directoryTree_ = dirTree;
}

int DirectoryProperty::getVariantType() {
    return Variant::VariantTypeString;
}

Variant DirectoryProperty::getVariant() {
    return Variant(get());
}

void  DirectoryProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getString());
    }
}

void DirectoryProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;    
    s.serialize("directory", get());
}

void DirectoryProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string directory;
    d.deserialize("directory", directory);   
    set(directory);
}

} // namespace
