#ifndef IVW_INTMINMAXPROPERTYWIDGETQT_H
#define IVW_INTMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>
#include <QSpinBox>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntMinMaxPropertyWidgetQt(IntMinMaxProperty* property);

    void updateFromProperty();

private:
    IntMinMaxProperty* property_;
    RangeSliderQt* slider_;
    QSpinBox* spinBoxMin_;
    QSpinBox* spinBoxMax_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;
    ivec2 valueMinMaxMax_;
    ivec2 valueMinMaxMin_;
    ivec2 valueIncrement_;
    ivec2 valueMinMax_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H