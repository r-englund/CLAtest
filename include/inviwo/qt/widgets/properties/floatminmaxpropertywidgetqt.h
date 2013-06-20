#ifndef IVW_FLOATMINMAXPROPERTYWIDGETQT_H
#define IVW_FLOATMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QDoubleSpinBox>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatMinMaxPropertyWidgetQt(FloatMinMaxProperty* property);

    void updateFromProperty();

private:
    FloatMinMaxProperty* property_;
    RangeSliderQt* slider_;
    QDoubleSpinBox* spinBoxMin_;
    QDoubleSpinBox* spinBoxMax_;

    void generateWidget();

protected slots:
    void setPropertyValue();

    void updateFromSlider(int valMin, int valMax);
    void updateFromSpinBoxMin(double val);
    void updateFromSpinBoxMax(double val);

protected:
    void setSpinBoxDecimals(float increment);

private:
    float maxNumberOfValues_;
    int sliderMin_;
    int sliderMax_;
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H