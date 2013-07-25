#ifndef IVW_IMAGEEDITORPROPERTY_H
#define IVW_IMAGEEDITORPROPERTY_H

#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {
/** class ImageEditorProperty
*  A class for file representations.
*  Holds the value of the path to a file as a string.
*  
* @see FileProperty
*/

class ImageEditorProperty : public FileProperty {

public:

    /** 
     * \brief Constructor for the ImageFileProperty
     *
     * The PropertySemantics can be set to Editor. Then a ImageEditorWidget will be used instead of a FilePropertyWidget
     * 
     * @param std::string identifier identifier for the property 
     * @param std::string displayName displayName for the property
     * @param std::string value the path to the file
     * @param PropertySemantics::Type semantics Can be set to Editor
     */
    ImageEditorProperty(std::string identifier, std::string displayName, std::string value,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT, 
                 PropertySemantics::Type semantics = PropertySemantics::Editor);
    virtual std::string getClassName()  const { return "ImageEditorProperty"; }
};

} // namespace

#endif // IVW_IMAGEEDITORPROPERTY_H