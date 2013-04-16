#ifndef IVW_COLORPROPERTYWDIGETQT_H
#define IVW_COLORPROPERTYWDIGETQT_H

#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API ColorPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:

    ColorPropertyWidgetQt(Property* property);

    void updateFromProperty();
    QColor getCurrentColor();

private:
    Property* property_;
    QPushButton* btnColor_;
    QColorDialog* colorDialog_;
    QColor* currentColor_;
    void generateWidget();
    
public slots:
    void setPropertyValue();
    void openColorDialog();
};

}//namespace

#endif //IVW_COLORPROPERTYWIDGETQT_H