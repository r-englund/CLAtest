#ifndef IVW_COMPOSITEPROPERTYWIDGETQT_H
#define IVW_COMPOSITEPROPERTYWIDGETQT_H

#include <QLineEdit>
#include <QToolButton>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/compositeproperty.h"

namespace inviwo {

class CompositePropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    CompositePropertyWidgetQt(CompositeProperty* property);

    void updateFromProperty();

private:
    CompositeProperty* property_;

    void generateWidget();

    std::vector<PropertyWidgetQt*> subPropertyWidgets_;
};

} // namespace

#endif // IVW_COMPOSITEPROPERTYWIDGETQT_H