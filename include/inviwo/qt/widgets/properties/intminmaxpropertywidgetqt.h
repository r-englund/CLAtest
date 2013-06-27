#ifndef IVW_INTMINMAXPROPERTYWIDGETQT_H
#define IVW_INTMINMAXPROPERTYWIDGETQT_H
//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QSpinBox>
//Core
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntMinMaxPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntMinMaxPropertyWidgetQt(IntMinMaxProperty* property);

    void updateFromProperty();

private:
    IntMinMaxProperty* property_;
    RangeSliderQt* slider_;
    QSpinBox* spinBoxMin_;
    QSpinBox* spinBoxMax_;

    bool updatingFromProperty_;

    void generateWidget();

protected slots:
    void setPropertyValue();

    void updateFromSlider(int valMin, int valMax);
    void updateFromSpinBoxMin(int val);
    void updateFromSpinBoxMax(int val);
};

} // namespace

#endif // IVW_INTMINMAXPROPERTYWIDGETQT_H