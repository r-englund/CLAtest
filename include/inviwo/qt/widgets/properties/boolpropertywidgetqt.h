#ifndef IVW_BOOLPROPERTYWIDGETQT_H
#define IVW_BOOLPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/boolproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API BoolPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    BoolPropertyWidgetQt(BoolProperty* property);

    void updateFromProperty();

private:
    BoolProperty* property_;
    QCheckBox* checkBox_;

    void generateWidget();

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_BOOLPROPERTYWIDGETQT_H