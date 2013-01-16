#ifndef IVW_FLOATPROPERTYWIDGETQT_H
#define IVW_FLOATPROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/scalarproperties.h"

namespace inviwo {

class FloatPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    FloatPropertyWidgetQt(FloatProperty* property);

    void updateFromProperty();

private:
    FloatProperty* property_;
    QSlider* slider_;

    void generateWidget();

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_FLOATPROPERTYWIDGETQT_H