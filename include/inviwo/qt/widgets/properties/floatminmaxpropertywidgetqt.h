#ifndef IVW_FLOATMINMAXPROPERTYWIDGETQT_H
#define IVW_FLOATMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/rangefloatsliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty* property);

    void updateFromProperty();

private:
    FloatMinMaxProperty* property_;
    RangeFloatSliderQt* slider_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;
    vec2 valueMinMaxMax_;
    vec2 valueMinMaxMin_;
    vec2 valueIncrement_;
    vec2 valueMinMax_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H