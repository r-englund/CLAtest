#include <inviwo/core/properties/imageeditorproperty.h>

namespace inviwo {

ImageLabel::ImageLabel() : startPoint_(0), rectSize_(0), name_("") {

}

ImageLabel::ImageLabel(glm::vec2 startPoint, glm::vec2 rectSize, std::string name) :
    startPoint_(startPoint), 
    rectSize_(rectSize), 
    name_(name) {
}

void ImageLabel::serialize(IvwSerializer& s) const {    
    s.serialize("labelName", name_, true);
    s.serialize("topLeft", startPoint_);
    s.serialize("size", rectSize_);
}

void ImageLabel::deserialize(IvwDeserializer& d) {    
    d.deserialize("labelName", name_, true);
    d.deserialize("topLeft", startPoint_);
    d.deserialize("size", rectSize_);
}


ImageEditorProperty::ImageEditorProperty(std::string identifier, std::string displayName,std::string value, PropertyOwner::InvalidationLevel invalidationLevel, 
PropertySemantics::Type semantics)
    : FileProperty(identifier, displayName, value, invalidationLevel, semantics)
{}

void ImageEditorProperty::setDimensions(glm::ivec2 imgSize) {    
   dimensions_ = imgSize;
}

void ImageEditorProperty::addLabel(glm::vec2 startPoint, glm::vec2 rectSize, std::string name) {    
    labels_.push_back(new ImageLabel(startPoint, rectSize, name));
}

std::vector<ImageLabel*> ImageEditorProperty::getLabels() const {    
    return labels_;
}

void ImageEditorProperty::clearLabels() {
    for (size_t i=0; i<labels_.size(); i++)
        delete labels_[i];
    labels_.clear();
}

void ImageEditorProperty::serialize(IvwSerializer& s) const {
    FileProperty::serialize(s);
    s.serialize("Dimension", dimensions_);
    s.serialize("Labels", labels_, "Label");
}

void ImageEditorProperty::deserialize(IvwDeserializer& d) {
    FileProperty::deserialize(d);
    d.deserialize("Dimension", dimensions_);
    d.deserialize("Labels", labels_, "Label");
}


} // namespace
