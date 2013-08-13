#ifndef IVW_EVENTPROPERTYMANAGERWIDGET_H
#define IVW_EVENTPROPERTYMANAGERWIDGET_H

#include <inviwo/core/util/observer.h>
#include <inviwo/qt/editor/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QVBoxLayout>
#include <QWidget>

namespace inviwo {

class IVW_QTEDITOR_API EventPropertyManagerWidget : public QWidget, public VoidObserver {
	Q_OBJECT

public:
	EventPropertyManagerWidget(EventPropertyManager* eventPropertyManager);
	~EventPropertyManagerWidget();	

	void notify();
private:
	void emptyLayout(QVBoxLayout* layout);
	void drawEventPropertyWidgets();	

	EventPropertyManager* eventPropertyManager_;
	QVBoxLayout* mainLayout_;
	std::map<std::string, bool> groupCollapsed;
};

} //namespace

#endif // IVW_EVENTPROPERTYMANAGERWIDGET_H