#ifndef IVW_PROCESSORPROPERTYWIDGET_H
#define IVW_PROCESSORPROPERTYWIDGET_H

#include <QWidget>

namespace inviwo {

class ProcessorPropertyWidget : public QWidget {

public:
    ProcessorPropertyWidget(QWidget* parent);
    ~ProcessorPropertyWidget();
};

} // namespace

#endif // IVW_PROCESSORPROPERTYWIDGET_H