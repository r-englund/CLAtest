#include <inviwo/core/properties/snapshotproperty.h>

namespace inviwo {

    SnapshotProperty::SnapshotProperty(std::string identifier, std::string displayName, ImagePort& imagePort)
: TemplateProperty<std::string>(identifier, displayName, "")
{
    imagePort_ = &imagePort;
}

SnapshotProperty::~SnapshotProperty() {}

void SnapshotProperty::saveSnapshot() {
    // TODO: Might be good to do some validation of the ImagePort.
    // Perhaps check whether it's an input or output port?


    // TODO: Implement real saving functionality
    if (!value_.empty()) {
        std::cout << "Snapshot being saved..." << std::endl;
    } else {
        std::cout << "No filename specified..." << std::endl;
    }
}

} // namespace
