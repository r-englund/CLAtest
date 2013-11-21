#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/eventpropertymanagerwidget.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QComboBox>

namespace inviwo {

class EventPropertyManagerWidget;

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public VoidObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void notify();	

private:
	void updateWidget();
	void buildLayout();
	std::vector<Processor*> findProcessorsWithInteractionHandlers(std::vector<Processor*> processors);

	ProcessorNetwork* processorNetwork_;
	EventPropertyManager* eventPropertyManager_;
	std::vector<Processor*> processorsWithInteractionHandlers_;
	std::vector<Processor*> prevProcessorsWithInteractionHandlers_;

	QComboBox* comboBox_;

	int currentIndex_;
public slots:
	void comboBoxChange();

};

} // namespace

#endif // IVW_MAPPINGWIDGET_H