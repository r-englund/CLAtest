#include <inviwo/core/properties/snapshotproperty.h>
#include <inviwo/core/io/imageloader.h>
namespace inviwo {

    SnapshotProperty::SnapshotProperty(std::string identifier, std::string displayName, PropertyOwner::InvalidationLevel invalidationLevel,
                                       PropertySemantics::Type semantics)
: TemplateProperty<std::string>(identifier, displayName, "", invalidationLevel, semantics)
{
}

SnapshotProperty::~SnapshotProperty() {}

void SnapshotProperty::saveSnapshot() {
    if (value_.size() != 0)
        saveSnapshot(value_.c_str(), image_);

}

void SnapshotProperty::saveSnapshot(const char* path, const Image* image){
    if (sizeof(path) != 0){
        ImageLoader::saveImage(path, image);
    } else {
        std::cout << "No filename specified..." << std::endl;
    } 
}

} // namespace
