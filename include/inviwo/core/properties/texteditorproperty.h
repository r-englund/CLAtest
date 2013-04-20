#ifndef IVW_TEXTEDITORPROPERTY_H
#define IVW_TEXTEDITORPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

class TextEditorProperty : public TemplateProperty<std::string> {

public:
    TextEditorProperty(std::string identifier, std::string displayName, std::string value,
                       PropertyOwner::InvalidationLevel invalidationLevel,
                       PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};



} // namespace

#endif // IVW_TEXTEDITORPROPERTY_H