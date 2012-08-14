#ifndef IVW_INVIWODOCKWIDGET_H
#define IVW_INVIWODOCKWIDGET_H

#include <QDockWidget>

namespace inviwo {

class InviwoDockWidget : public QDockWidget {

public:
    InviwoDockWidget(QString title, QWidget* parent);
    ~InviwoDockWidget();
};

} // namespace

#endif // IVW_INVIWODOCKWIDGET_H