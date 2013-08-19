#ifndef IVW_INTVEC2PROPERTYWIDGETQT_H
#define IVW_INTVEC2PROPERTYWIDGETQT_H

#include <QMenu>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntVec2PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntVec2PropertyWidgetQt(IntVec2Property* property);

    void updateFromProperty();

private:
    IntVec2Property* property_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;
    IntSliderWidgetQt* sliderX_;
    IntSliderWidgetQt* sliderY_;
    ivec2 valueVecMax_;
    ivec2 valueVecMin_;
    ivec2 valueIncrement_;
    ivec2 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);

};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H