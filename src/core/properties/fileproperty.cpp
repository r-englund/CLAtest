#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/util/filedirectory.h>

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
    std::string basePath = s.getFileName();
    std::string absoluteFilePath = get();

    ivwAssert(!basePath.empty(), "File name cannot be empty.");
    
    std::string relativePath = UrlParser::getRelativePath(basePath, absoluteFilePath);
    s.serialize("url", relativePath);
}

void FileProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string relativePath;
    d.deserialize("url", relativePath);

    std::string basePath = d.getFileName();
    ivwAssert(!basePath.empty(), "File name cannot be empty.")
    
    basePath = UrlParser::getFileDirectory(basePath);
    set(basePath+relativePath);
}

} // namespace
