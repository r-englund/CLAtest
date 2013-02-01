#ifndef IVW_STRINGPROPERTY_H
#define IVW_STRINGPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

    class StringProperty : public TemplateProperty<std::string> {

    public:
        StringProperty(std::string identifier, std::string displayName, std::string value);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_STRINGPROPERTY_H