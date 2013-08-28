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
    
    void showWidget();
    void hideWidget();
public slots:
   void visibilityModified(int mode);
    
signals:
    void modified();
    void visibilityChange();
};

} // namespace

#endif // IVW_PROPERTYWIDGETQT_H