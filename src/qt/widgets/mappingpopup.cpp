#include <inviwo/qt/widgets/mappingpopup.h>

namespace inviwo {

MappingPopup::MappingPopup( EventProperty * parentProperty ) {
    parentProperty_ = parentProperty;
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent( QMouseEvent * event ) {
    parentProperty_->tmpTest((int)event->button());
    this->close();
}

} // namespace