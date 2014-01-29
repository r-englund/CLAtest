#ifndef IVW_SPHERICALFLOATVEC3PROPERTYWIDGETQT_H
#define IVW_SPHERICALFLOATVEC3PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/floatsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <QMenu>
#include <math.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API SphericalFloatVec3PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT

public:
    SphericalFloatVec3PropertyWidgetQt(FloatVec3Property* property);
    virtual ~SphericalFloatVec3PropertyWidgetQt(){}

    void updateFromProperty();

private:
    FloatVec3Property* property_;
    FloatSliderWidgetQt* sliderR_;
    FloatSliderWidgetQt* sliderTheta_;
    FloatSliderWidgetQt* sliderPhi_;
    QLabel* readOnlyLabel_;
    PropertySettingsWidgetQt* settingsWidget_;
    vec3 valueVec3Max_;
    vec3 valueVec3Min_;
    vec3 valueIncrement_;
    QMenu* settingsMenu_;
    EditableLabelQt* label_;

    void generateWidget();
    void generatesSettingsWidget();

    vec3 cartesianToSpherical(vec3);
    vec3 sphericalToCartesian(vec3);

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);


};

} // namespace

#endif // IVW_SPHERICALFLOATVEC3PROPERTYWIDGETQT_H

