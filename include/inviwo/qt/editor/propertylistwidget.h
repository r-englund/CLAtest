#ifndef IVW_PROPERTYLISTWIDGET_H
#define IVW_PROPERTYLISTWIDGET_H

#include "inviwo/qt/editor/inviwodockwidget.h"

namespace inviwo {

class PropertyListWidget : public InviwoDockWidget {

public:
    PropertyListWidget(QWidget* parent);
    ~PropertyListWidget();
};

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H