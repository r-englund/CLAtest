#ifndef IVW_OPTIONPROPERTIES_H
#define IVW_OPTIONPROPERTIES_H

#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {
    class IVW_CORE_API OptionPropertyDouble : public TemplatedOptionProperty<double> {

    public:
        OptionPropertyDouble(std::string identifier, std::string displayName, std::string value, int selectedOption);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

    class IVW_CORE_API OptionPropertyFloat : public TemplatedOptionProperty<float> {

    public:
        OptionPropertyFloat(std::string identifier, std::string displayName, std::string value, int selectedOption);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

    class IVW_CORE_API OptionPropertyInt : public TemplatedOptionProperty<int> {

    public:
        OptionPropertyInt(std::string identifier, std::string displayName, std::string value, int selectedOption);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

    class IVW_CORE_API OptionPropertyString : public TemplatedOptionProperty<std::string> {

    public:
        OptionPropertyString(std::string identifier, std::string displayName, std::string value, int selectedOption);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_OPTIONPROPERTIES_H