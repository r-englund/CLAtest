#ifndef IVW_PROPERTYWIDGET_H
#define IVW_PROPERTYWIDGET_H

namespace inviwo {

class PropertyWidget {

public:
    PropertyWidget();

    //virtual PropertyWidget* create();
    virtual void updateFromProperty() = 0;
};

} // namespace

#endif // IVW_PROPERTYWIDGET_H