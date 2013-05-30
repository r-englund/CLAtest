#ifndef IVW_OPTIONPROPERTIES_H
#define IVW_OPTIONPROPERTIES_H

#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

/** class OptionPropertyInt
* 
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyInt : public TemplateOptionProperty<int> {

public:
    OptionPropertyInt(std::string identifier, std::string displayName,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics=PropertySemantics::Default);
    OptionPropertyInt(std::string identifier, std::string displayName, int value,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};


/** class OptionPropertyFloat
* 
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyFloat : public TemplateOptionProperty<float> {

public:
    OptionPropertyFloat(std::string identifier, std::string displayName,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics::Type semantics=PropertySemantics::Default);
    OptionPropertyFloat(std::string identifier, std::string displayName, float value,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};


/** class OptionPropertyDouble
* 
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyDouble : public TemplateOptionProperty<double> {

public:
    OptionPropertyDouble(std::string identifier, std::string displayName,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics::Type semantics=PropertySemantics::Default);
    OptionPropertyDouble(std::string identifier, std::string displayName, double value,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};


/** class OptionPropertyString
* 
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyString : public TemplateOptionProperty<std::string> {

public:
    OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics::Type semantics=PropertySemantics::Default);
    OptionPropertyString(std::string identifier, std::string displayName,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics::Type semantics=PropertySemantics::Default);
    void addOption(std::string identifier, std::string displayName);
    void addOption(std::string identifier, std::string displayName, std::string value);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_OPTIONPROPERTIES_H