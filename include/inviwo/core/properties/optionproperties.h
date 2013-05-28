#ifndef IVW_OPTIONPROPERTIES_H
#define IVW_OPTIONPROPERTIES_H

#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {
/** class OptionPropertyDouble
* 
* @see TemplatedOptionProperty
*/
class IVW_CORE_API OptionPropertyDouble : public TemplatedOptionProperty<double> {

public:
    OptionPropertyDouble(std::string identifier, std::string displayName, std::string value,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/** class OptionPropertyFloat
* 
* @see TemplatedOptionProperty
*/
class IVW_CORE_API OptionPropertyFloat : public TemplatedOptionProperty<float> {

public:
    OptionPropertyFloat(std::string identifier, std::string displayName, std::string value,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};
/** class OptionPropertyInt
* 
* @see TemplatedOptionProperty
*/
class IVW_CORE_API OptionPropertyInt : public TemplatedOptionProperty<int> {

public:
    OptionPropertyInt(std::string identifier, std::string displayName, std::string value,
                       PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                       PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};
/** class OptionPropertyString
* 
* @see TemplatedOptionProperty
*/
class IVW_CORE_API OptionPropertyString : public TemplatedOptionProperty<std::string> {

public:
    OptionPropertyString(std::string identifier, std::string displayName, std::string value,
        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
        PropertySemantics::Type semantics = PropertySemantics::Default);
    void addOption(std::string optionIdentifier, std::string optionDisplayName);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_OPTIONPROPERTIES_H