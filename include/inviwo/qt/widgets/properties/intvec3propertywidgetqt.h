#ifndef IVW_INTVEC3PROPERTYWIDGETQT_H
#define IVW_INTVEC3PROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

class IVW_QTWIDGETS_API IntVec3PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntVec3PropertyWidgetQt(IntVec3Property* property);

    void updateFromProperty();

private:
    IntVec3Property* property_;
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

#endif // IVW_INTVEC2PROPERTYWIDGETQT_H