#ifndef IVW_STRINGOPTIONPROPERTY_H
#define IVW_STRINGOPTIONPROPERTY_H

#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

    class IVW_CORE_API StringOptionProperty : public TemplatedOptionProperty<std::string> {

    public:
        StringOptionProperty(std::string identifier, std::string displayName, std::string value, int selectedOption);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_STRINGOPTIONPROPERTY_H