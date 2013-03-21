#ifndef IVW_FLOATPROPERTYWIDGETQT_H
#define IVW_FLOATPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>
#include <QMenu>
#include <QDoubleSpinBox>
#include <inviwo/qt/widgets/floatsliderqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/scalarproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatPropertyWidgetQt(FloatProperty* property);

    void updateFromProperty();

private:
    FloatProperty* property_;
    //QSlider* slider_;
    FloatSliderQt* slider_;
    QDoubleSpinBox* spinBox_;
    PropertySettingsWidgetQt* settingsWidget_;

    void generateWidget();

public slots:
    void setPropertyValueFromSlider();
    void setPropertyValueFromSpinbBox();

    void showContextMenu(const QPoint& pos);
};

} // namespace

#endif // IVW_FLOATPROPERTYWIDGETQT_H