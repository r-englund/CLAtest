#ifndef IVW_IMAGEEDITORPROPERTY_H
#define IVW_IMAGEEDITORPROPERTY_H

#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {

class ImageLabel : public IvwSerializable {
public :
    ImageLabel();
    ImageLabel(glm::vec2 startPoint, glm::vec2 rectSize, std::string name);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    std::string getName() const {return name_;}
    glm::vec2 getTopLeft() const {return startPoint_;}
    glm::vec2 getSize() const {return rectSize_;}
private:
    std::string name_;
    glm::vec2 startPoint_;
    glm::vec2 rectSize_;
};

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
    void addLabel(glm::vec2 start, glm::vec2 end, std::string name="");
    void setDimensions(glm::ivec2 imgSize);
    std::vector<ImageLabel*> getLabels() const;
    void clearLabels();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);    
private:
    std::vector<ImageLabel*> labels_;
    glm::ivec2 dimensions_;
};

} // namespace

#endif // IVW_IMAGEEDITORPROPERTY_H