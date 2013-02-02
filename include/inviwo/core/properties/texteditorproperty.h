#ifndef IVW_TEXTEDITORPROPERTY_H
#define IVW_TEXTEDITORPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

    class TextEditorProperty : public TemplateProperty<std::string> {

    public:
        TextEditorProperty(std::string identifier, std::string displayName, std::string value);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_TEXTEDITORPROPERTY_H