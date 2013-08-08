#include <inviwo/qt/widgets/mappingpopup.h>
#include <apps/inviwo/inviwomainwindow.h>


namespace inviwo {

MappingPopup::MappingPopup(EventProperty* eventProperty, EventPropertyManager* parentManager ) {
    parentManager_ = parentManager;
	eventProperty_ = eventProperty;
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent( QMouseEvent * event ) {	
    parentManager_->changeMouseMapping(eventProperty_, EventConverterQt::getMouseButton(event), EventConverterQt::getModifier(event));

	InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());    
	InviwoMainWindow* win = static_cast<InviwoMainWindow*>(appQt->getMainWindow());
	MappingWidget* mappingWidget = win->findChild<MappingWidget*>();
	mappingWidget->updateWidget();
}

void MappingPopup::keyReleaseEvent( QKeyEvent * event ){
	char button = EventConverterQt::getKeyButton(event);	
	if (button > 0) {
		parentManager_->changeKeyMapping(eventProperty_, button, EventConverterQt::getModifier(event));

		InviwoApplicationQt* appQt = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());    
		InviwoMainWindow* win = static_cast<InviwoMainWindow*>(appQt->getMainWindow());
		MappingWidget* mappingWidget = win->findChild<MappingWidget*>();
		mappingWidget->updateWidget();

		this->close();
	}
}

void MappingPopup::mouseReleaseEvent(QMouseEvent * event) {
	this->close();
}

} // namespace