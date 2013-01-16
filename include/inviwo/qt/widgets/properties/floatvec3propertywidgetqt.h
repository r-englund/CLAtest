#ifndef IVW_FLOATVEC3PROPERTYWIDGETQT_H
#define IVW_FLOATVEC3PROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

class FloatVec3PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatVec3PropertyWidgetQt(FloatVec3Property* property);

    void updateFromProperty();

private:
    FloatVec3Property* property_;
    QSlider* sliderX_;
    QSlider* sliderY_;
    QSlider* sliderZ_;

    void generateWidget();

public slots:
    void setXValue(int value);
    void setYValue(int value);
    void setZValue(int value);
    void setPropertyValue();
};

} // namespace

#endif // IVW_FLOATVEC3PROPERTYWIDGETQT_H