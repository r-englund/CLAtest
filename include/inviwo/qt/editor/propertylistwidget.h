#ifndef IVW_PROPERTYLISTWIDGET_H
#define IVW_PROPERTYLISTWIDGET_H

#include <QDockWidget>

namespace inviwo {

class PropertyListWidget : public QDockWidget {

public:
    PropertyListWidget(QWidget* parent);
    ~PropertyListWidget();
};

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H