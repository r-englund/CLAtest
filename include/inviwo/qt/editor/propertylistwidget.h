#ifndef IVW_PROPERTYLISTWIDGET_H
#define IVW_PROPERTYLISTWIDGET_H

#include "inviwo/core/processors/processor.h"

#include "inviwo/qt/editor/inviwodockwidget.h"
#include "inviwo/qt/widgets/properties/propertywidgetfactoryqt.h"
#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

namespace inviwo {

class PropertyListWidget : public InviwoDockWidget {

public:
    static PropertyListWidget* instance();

    PropertyListWidget(QWidget* parent);
    ~PropertyListWidget();

    void showProcessorProperties(Processor* processor);
    void removeProcessorProperties(Processor* processor);
    void showProcessorProperties(std::vector<Processor*> processors);
    void removeAllProcessorProperties();

private:
    QWidget* createNewProcessorPropertiesItem(Processor* processor);
    void addProcessorPropertiesToLayout(Processor* processor);

    QVBoxLayout* listWidgetLayout_;
    QWidget* listWidget_;

protected:
    static PropertyListWidget* propertyListWidget_;
    PropertyWidgetFactoryQt* propertyWidgetFactory_;
    mutable std::map<std::string, QWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H