#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/editor/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>

namespace inviwo {

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public VoidObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void notify();
	void updateWidget();

private:
	void drawEventPropertyWidgets();	
	void emptyLayout(QVBoxLayout* layout);
	void removeEventPropertyWidgets();
	void buildLayout();
	std::vector<Processor*> findProcessorsWithInteractionHandlers(std::vector<Processor*> processors);

	ProcessorNetwork* processorNetwork_;
	EventPropertyManager* eventPropertyManager_;
	std::vector<Processor*> processorsWithInteractionHandlers_;
	std::vector<Processor*> prevProcessorsWithInteractionHandlers_;

	QVBoxLayout *botLayout_;
	QComboBox* comboBox_;
	QVBoxLayout* mainLayout;

	int currentIndex_;
public slots:
	void comboBoxChange();

};

} // namespace

#endif // IVW_MAPPINGWIDGET_H