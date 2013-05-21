#ifndef IVW_PROPERTYWIDGETQT_H
#define IVW_PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include <inviwo/core/properties/propertywidget.h>

namespace inviwo {

class IVW_QTWIDGETS_API PropertyWidgetQt : public QWidget, public PropertyWidget {

Q_OBJECT

public:
    PropertyWidgetQt();
    PropertyWidgetQt* create();

signals:
    void modified();
};

} // namespace

#endif // IVW_PROPERTYWIDGETQT_H