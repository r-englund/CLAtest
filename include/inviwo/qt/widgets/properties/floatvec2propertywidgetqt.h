#ifndef IVW_FLOATVEC2PROPERTYWIDGETQT_H
#define IVW_FLOATVEC2PROPERTYWIDGETQT_H

#include <QSlider>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatVec2PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatVec2PropertyWidgetQt(FloatVec2Property* property);

    void updateFromProperty();

private:
    FloatVec2Property* property_;
    QSlider* sliderX_;
    QSlider* sliderY_;

    void generateWidget();

public slots:
    void setXValue(int value);
    void setYValue(int value);
    void setPropertyValue();
};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H