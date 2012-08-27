#ifndef IVW_INTPROPERTYWIDGETQT_H
#define IVW_INTPROPERTYWIDGETQT_H

#include <QSlider>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/scalarproperties.h"

namespace inviwo {

class IntPropertyWidgetQt : public PropertyWidgetQt {

Q_OBJECT;

public:
    IntPropertyWidgetQt(IntProperty* property);

    void updateFromProperty();

private:
    IntProperty* property_;
    QSlider* slider_;

    void generateWidget();

    public slots:
        void setPropertyValue(int value);
};

} // namespace

#endif // IVW_INTPROPERTYWIDGETQT_H