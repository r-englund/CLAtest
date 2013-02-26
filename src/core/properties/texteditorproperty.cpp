#include <inviwo/core/properties/texteditorproperty.h>

namespace inviwo {

    TextEditorProperty::TextEditorProperty(std::string identifier, std::string displayName, std::string value, PropertySemantics::Type semantics /*= PropertySemantics::Default*/)
        : TemplateProperty<std::string>(identifier, displayName, value, semantics)
    {}

    void TextEditorProperty::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("value", get());
    }

    void TextEditorProperty::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        std::string value;
        d.deserialize("value", value);
        set(value);
    }

} // namespace
