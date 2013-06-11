#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/editor/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

namespace inviwo {

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public VoidObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void notify();

private:
	ProcessorNetwork* processorNetwork_;
	EventPropertyManager* eventPropertyManager_;
	std::vector<Processor*> curProcessorList_;

    QLabel* label_;
	QVBoxLayout* vLayout_;
	QFrame* frame_;

	void drawEventPropertyWidgets();
    void updateWidget();
	void emptyLayout();

    QString intToQString(int num); // For testing
};

} // namespace

#endif // IVW_MAPPINGWIDGET_H