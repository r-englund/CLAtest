#ifndef IVW_CONSOLEWIDGET_H
#define IVW_CONSOLEWIDGET_H

#include "inviwo/qt/editor/inviwodockwidget.h"

namespace inviwo {

class ConsoleWidget : public InviwoDockWidget {

public:
    ConsoleWidget(QWidget* parent);
    ~ConsoleWidget();
};

} // namespace

#endif // IVW_CONSOLELISTWIDGET_H