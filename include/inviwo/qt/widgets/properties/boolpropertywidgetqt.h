#ifndef IVW_BOOLPROPERTYWIDGETQT_H
#define IVW_BOOLPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
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
    EditableLabelQt* label_;
    void generateWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
    void visibilityModified(int mode){property_->setVisibility(static_cast<PropertyVisibility::VisibilityMode>(mode));};
};

} // namespace

#endif // IVW_BOOLPROPERTYWIDGETQT_H