#ifndef IVW_PROCESSORPROPERTYWIDGET_H
#define IVW_PROCESSORPROPERTYWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QWidget>

namespace inviwo {

class IVW_QTEDITOR_API ProcessorPropertyWidget : public QWidget {

public:
    ProcessorPropertyWidget(QWidget* parent);
    ~ProcessorPropertyWidget();
};

} // namespace

#endif // IVW_PROCESSORPROPERTYWIDGET_H