#ifndef IVW_CONSOLEWIDGET_H
#define IVW_CONSOLEWIDGET_H

#include <QDockWidget>

namespace inviwo {

class ConsoleWidget : public QDockWidget {

public:
    ConsoleWidget(QWidget* parent);
    ~ConsoleWidget();
};

} // namespace

#endif // IVW_CONSOLELISTWIDGET_H