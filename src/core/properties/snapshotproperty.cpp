#include <inviwo/core/properties/snapshotproperty.h>
#include <inviwo/core/io/imageloader.h>
namespace inviwo {

    SnapshotProperty::SnapshotProperty(std::string identifier, std::string displayName, ImagePort& imagePort,
                                       PropertyOwner::InvalidationLevel invalidationLevel,
                                       PropertySemantics::Type semantics)
: TemplateProperty<std::string>(identifier, displayName, "", invalidationLevel, semantics)
{
    imagePort_ = &imagePort;
}

SnapshotProperty::~SnapshotProperty() {}

void SnapshotProperty::saveSnapshot() {
    // Perhaps check whether it's an input or output port?
    const Image *image = imagePort_->getData();
    if(value_.size() != 0)
        saveSnapshot(value_.c_str(), image);

}

void SnapshotProperty::saveSnapshot(const char* path, const Image* image){
    if(sizeof(path) != 0){
        ImageLoader::saveImage(path, image);
    } else {
        std::cout << "No filename specified..." << std::endl;
    } 
}

} // namespace
