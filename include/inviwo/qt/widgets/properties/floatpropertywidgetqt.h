#ifndef IVW_FLOATPROPERTYWIDGETQT_H
#define IVW_FLOATPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QMenu>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
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
    FloatSliderWidgetQt* sliderWidget_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValueFromSlider();
    void setPropertyValueFromSpinBox();

    void showContextMenu(const QPoint& pos);
};

} // namespace

#endif // IVW_FLOATPROPERTYWIDGETQT_H