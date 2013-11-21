#ifndef IVW_EVENTPROPERTYWIDGETQT_H
#define IVW_EVENTPROPERTYWIDGETQT_H

#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/mappingpopup.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

namespace inviwo {

class IVW_QTWIDGETS_API EventPropertyWidgetQt : public PropertyWidgetQt {
    Q_OBJECT

public:
    EventPropertyWidgetQt(EventProperty* eventproperty);
    void updateFromProperty();
	void setManager(EventPropertyManager* eventPropertyManager) { eventPropertyManager_ = eventPropertyManager; };

private:
    EventProperty* eventproperty_;
    QPushButton* button_;
	EventPropertyManager* eventPropertyManager_;

    void generateWidget();

public slots:
    void clickedSlot();
};

} //namespace

#endif // IVW_EVENTPROPERTYWIDGETQT_H