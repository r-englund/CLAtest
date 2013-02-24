#ifndef IVW_KEYMAPPROPERTY_H
#define IVW_KEYMAPPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

    class IVW_CORE_API KeyMapProperty : public TemplateProperty<std::string> {

    public:
        KeyMapProperty(std::string identifier, std::string displayName, std::string value);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_KEYMAPPROPERTY_H