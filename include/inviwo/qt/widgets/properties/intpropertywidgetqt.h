#ifndef IVW_INTPROPERTYWIDGETQT_H
#define IVW_INTPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/intsliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <QMenu>

namespace inviwo {

class IVW_QTWIDGETS_API IntPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntPropertyWidgetQt(IntProperty* property);

    void updateFromProperty();

private:
    IntProperty* property_;
    IntSliderWidgetQt* sliderWidget_;
    PropertySettingsWidgetQt* settingsWidget_;
    QMenu* settingsMenu_;

    void generateWidget();
    void generatesSettingsWidget();

public slots:
    void setPropertyValue();

    void showContextMenu(const QPoint& pos);
};

} // namespace

#endif // IVW_INTPROPERTYWIDGETQT_H