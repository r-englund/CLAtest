#include <inviwo/qt/widgets/mappingpopup.h>

namespace inviwo {

MappingPopup::MappingPopup(EventProperty* eventProperty, EventPropertyManager* parentManager ) {
    parentManager_ = parentManager;
	eventProperty_ = eventProperty;
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent( QMouseEvent * event ) {	
    parentManager_->changeMouseMapping(eventProperty_, EventConverterQt::getMouseButton(event), EventConverterQt::getModifier(event));
	InviwoApplication::getPtr()->getProcessorNetwork()->notifyObservers();
    this->close();
}

void MappingPopup::keyReleaseEvent( QKeyEvent * event ){
	char button = EventConverterQt::getKeyButton(event);	
	if (button > 0) {
		parentManager_->changeKeyMapping(eventProperty_, button, EventConverterQt::getModifier(event));
		InviwoApplication::getPtr()->getProcessorNetwork()->notifyObservers();
		this->close();
	}
}

} // namespace