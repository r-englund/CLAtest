#ifndef IVW_INTVEC4PROPERTYWIDGETQT_H
#define IVW_INTVEC4PROPERTYWIDGETQT_H

#include <QMenu>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntVec4PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntVec4PropertyWidgetQt(IntVec4Property* property);

    void updateFromProperty();

private:
    IntVec4Property* property_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;
    IntSliderWidgetQt* sliderX_;
    IntSliderWidgetQt* sliderY_;
	IntSliderWidgetQt* sliderZ_;
	IntSliderWidgetQt* sliderW_;
    ivec4 valueVecMax_;
    ivec4 valueVecMin_;
    ivec4 valueIncrement_;
    ivec4 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValueFromSlider();
    void setPropertyValueFromSpinBox();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);
    void showContextMenuW(const QPoint& pos);


};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H

