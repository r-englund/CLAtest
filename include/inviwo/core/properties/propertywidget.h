#ifndef IVW_PROPERTYWIDGET_H
#define IVW_PROPERTYWIDGET_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

class IVW_CORE_API PropertyWidget {

public:
    PropertyWidget();

    //virtual PropertyWidget* create();
    virtual void updateFromProperty() = 0;
    virtual void showWidget() = 0;
    virtual void hideWidget() = 0;
};

} // namespace

#endif // IVW_PROPERTYWIDGET_H