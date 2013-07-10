#ifndef IVW_TEXTEDITORPROPERTY_H
#define IVW_TEXTEDITORPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

class TextEditorProperty : public TemplateProperty<std::string> {

public:
    /** 
     * \brief deprecated
     *
     * TextEditorProperty is not used anymore, to get a texteditor use fileproperty or stringproperty combined with PropertySemantics::Editor.
     * 
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param std::string value <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics::Type semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */TextEditorProperty(std::string identifier, std::string displayName, std::string value,
                       PropertyOwner::InvalidationLevel invalidationLevel,
                       PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "TextEditorProperty"; }
};



} // namespace

#endif // IVW_TEXTEDITORPROPERTY_H