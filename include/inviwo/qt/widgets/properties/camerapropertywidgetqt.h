#ifndef IVW_CAMERAPROPERTYWIDGETQT_H
#define IVW_CAMERAPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QSlider>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API CameraPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    CameraPropertyWidgetQt(CameraProperty* property);
    void updateFromProperty();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();

private:
    CameraProperty* property_;
    QSlider* slider_;
    EditableLabelQt* label_;

    void generateWidget();
};

} // namespace

#endif // IVW_CAMERAPROPERTYWIDGETQT_H