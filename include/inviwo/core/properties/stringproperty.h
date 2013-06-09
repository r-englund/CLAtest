#ifndef IVW_STRINGPROPERTY_H
#define IVW_STRINGPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

    /** class StringProperty
    * \brief  The StringProperty holds a string as value.
    * 
    * The string property has 2 different widgets, one that just displays the value of the string.
    * The other one makes it possible to edit the string.
    * To use the editor widget you must specify it in the constructor and set the property semantics to Editor.
    * 
    * @see StringPropertyWidgetQt
    * @see TextEditorWidgetQt
    */

class IVW_CORE_API StringProperty : public TemplateProperty<std::string> {

public:
    /**
     * \brief Constructor for the StringProperty
     *
     * The PropertySemantics can be set to Editor. Then a TextEditorWidget will be used instead of a StringPropertyWidget
     *
     *
     * @see StringPropertyWidgetQt
     * @see TextEditorWidgetQt
     * @param std::string identifier identifier for the property
     * @param std::string displayName displayName for the property
     * @param std::string value value of the property
     * @param PropertySemantics::Type semantics Semantics of the property, if nothing is specified it will set to the Default. The semantics can be specified as Editor
     * @return 
     */
    StringProperty(std::string identifier, std::string displayName, std::string value,
                   PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
                   PropertySemantics::Type semantics=PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_STRINGPROPERTY_H