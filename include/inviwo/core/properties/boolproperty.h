#ifndef IVW_BOOLPROPERTY_H
#define IVW_BOOLPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {
    /** class BoolProperty
    *\brief The BoolProperty class provides a property holding boolean values
    * 
    * A bool property are normally used by a processor.
    * The bool property is created and added in the constructor of the processor.
    * The propertys widget consist of a checkbox that sets the value of the property.
    * @see TemplateProperty
    * @see BoolPropertyWidgetQt
    */

class IVW_CORE_API BoolProperty : public TemplateProperty<bool> {

public:
    /** 
     * \brief Constructs a bool property
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param bool value <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics::Type semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */BoolProperty(std::string identifier, std::string displayName, bool value,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                 PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_BOOLPROPERTY_H