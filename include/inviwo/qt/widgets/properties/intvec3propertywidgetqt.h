#ifndef IVW_INTVEC3PROPERTYWIDGETQT_H
#define IVW_INTVEC3PROPERTYWIDGETQT_H

#include <QMenu>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntVec3PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntVec3PropertyWidgetQt(IntVec3Property* property);

    void updateFromProperty();

private:
    IntVec3Property* property_;
    PropertySettingsWidgetQt* settingsWidget_;
    EditableLabelQt* label_;
    QMenu* settingsMenu_;
    IntSliderWidgetQt* sliderX_;
    IntSliderWidgetQt* sliderY_;
    IntSliderWidgetQt* sliderZ_;
    ivec3 valueVecMax_;
    ivec3 valueVecMin_;
    ivec3 valueIncrement_;
    ivec3 valueVec_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
    void visibilityModified(int mode){property_->setVisibility(static_cast<PropertyVisibility::VisibilityMode>(mode));};

    void showContextMenuX(const QPoint& pos);
    void showContextMenuY(const QPoint& pos);
    void showContextMenuZ(const QPoint& pos);
};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H