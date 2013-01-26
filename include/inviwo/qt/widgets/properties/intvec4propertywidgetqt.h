#ifndef IVW_INTVEC4PROPERTYWIDGETQT_H
#define IVW_INTVEC4PROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

class IVW_QTWIDGETS_API IntVec4PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntVec4PropertyWidgetQt(IntVec4Property* property);

    void updateFromProperty();

private:
    IntVec4Property* property_;
    QSlider* sliderX_;
    QSlider* sliderY_;
	QSlider* sliderZ_;
	QSlider* sliderW_;

    void generateWidget();

public slots:
    void setXValue(int value);
    void setYValue(int value);
	void setZValue(int value);
	void setWValue(int value);
    void setPropertyValue();
};

} // namespace

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H

