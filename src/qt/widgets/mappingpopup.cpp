#include <inviwo/qt/widgets/mappingpopup.h>

namespace inviwo {

MappingPopup::MappingPopup(EventProperty* eventProperty, EventPropertyManager* parentManager ) {
    parentManager_ = parentManager;
	eventProperty_ = eventProperty;
    eventConverter_ = new EventConverterQt();
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent( QMouseEvent * event ) {
    parentManager_->changeKeybinding(eventProperty_, eventConverter_->getMouseButton(event));
	InviwoApplication::getPtr()->getProcessorNetwork()->notifyObservers();
    this->close();
}

} // namespace