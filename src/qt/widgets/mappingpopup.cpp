#include <inviwo/qt/widgets/mappingpopup.h>

namespace inviwo {

MappingPopup::MappingPopup( EventProperty * parentProperty ) {
    parentProperty_ = parentProperty;
    eventConverter_ = new EventConverterQt();
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent( QMouseEvent * event ) {
    parentProperty_->tmpTest(eventConverter_->getMouseButton(event));
    parentProperty_->updatePropertyWidgets();
    this->close();
}

} // namespace