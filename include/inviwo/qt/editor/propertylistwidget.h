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

protected:
    static PropertyListWidget* propertyListWidget_;
    PropertyWidgetFactoryQt* propertyWidgetFactory_;
    mutable std::map<std::string, QWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H