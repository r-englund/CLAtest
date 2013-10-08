#ifndef IVW_FLOATVEC2PROPERTYWIDGETQT_H
#define IVW_FLOATVEC2PROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QMenu>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatVec2PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatVec2PropertyWidgetQt(FloatVec2Property* property);

    void updateFromProperty();

private:
    FloatVec2Property* property_;
    FloatSliderWidgetQt* sliderX_;
    FloatSliderWidgetQt* sliderY_;
    QLabel* readOnlyLabel_;
    PropertySettingsWidgetQt* settingsWidget_;
    EditableLabelQt* label_;
    QMenu* settingsMenu_;
    vec2 valueVec2Max_;
    vec2 valueVec2Min_;
    vec2 valueIncrement_;
    vec2 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H