#ifndef IVW_FLOATVEC4PROPERTYWIDGETQT_H
#define IVW_FLOATVEC4PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatVec4PropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatVec4PropertyWidgetQt(FloatVec4Property* property);

    void updateFromProperty();

private:
    FloatVec4Property* property_;
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

#endif // IVW_FLOATVEC3PROPERTYWIDGETQT_H