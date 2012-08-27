#ifndef IVW_CAMERAPROPERTYWIDGETQT_H
#define IVW_CAMERAPROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/cameraproperty.h"

namespace inviwo {

class CameraPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    CameraPropertyWidgetQt(CameraProperty* property);
    void updateFromProperty();

public slots:
    void setPropertyValue(int value);

private:
    CameraProperty* property_;
    QSlider* slider_;

    void generateWidget();
};

} // namespace

#endif // IVW_CAMERAPROPERTYWIDGETQT_H