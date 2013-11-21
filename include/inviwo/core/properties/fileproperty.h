#ifndef IVW_FILEPROPERTY_H
#define IVW_FILEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {
/** class FileProperty
*  A class for file representations.
*  Holds the value of the path to a file as a string.
*  
* @see TemplateProperty
*/

class IVW_CORE_API FileProperty : public TemplateProperty<std::string> {

public:

    /** 
     * \brief Constructor for the FileProperty
     *
     * The PropertySemantics can be set to Editor. Then a TextEditorWidget will be used instead of a FilePropertyWidget
     * 
     * @param std::string identifier identifier for the property 
     * @param std::string displayName displayName for the property
     * @param std::string value the path to the file
     * @param PropertySemantics::Type semantics Can be set to Editor
     */
    FileProperty(std::string identifier, std::string displayName, std::string value="",
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT, 
                 PropertySemantics::Type semantics = PropertySemantics::Default);
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FileProperty"; }
};

} // namespace

#endif // IVW_FILEPROPERTY_H