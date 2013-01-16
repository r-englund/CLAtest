#ifndef IVW_INVIWODOCKWIDGET_H
#define IVW_INVIWODOCKWIDGET_H

#include "inviwo/qt/editor/inviwoqteditordefine.h"
#include <QDockWidget>

namespace inviwo {

class IVW_QTEDITOR_API InviwoDockWidget : public QDockWidget {

public:
    InviwoDockWidget(QString title, QWidget* parent);
    ~InviwoDockWidget();
};

} // namespace

#endif // IVW_INVIWODOCKWIDGET_H