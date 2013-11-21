#ifndef IVW_INVIWODOCKWIDGET_H
#define IVW_INVIWODOCKWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QDockWidget>

namespace inviwo {

class IVW_QTWIDGETS_API InviwoDockWidget : public QDockWidget {
    Q_OBJECT

public:
    InviwoDockWidget(QString title, QWidget* parent);
    ~InviwoDockWidget();
};

} // namespace

#endif // IVW_INVIWODOCKWIDGET_H