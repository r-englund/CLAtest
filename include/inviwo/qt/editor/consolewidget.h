#ifndef IVW_CONSOLEWIDGET_H
#define IVW_CONSOLEWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <QTextEdit>
#include <QKeyEvent>

namespace inviwo {

class IVW_QTEDITOR_API ConsoleWidget : public InviwoDockWidget, public Logger {
    Q_OBJECT
public:
    ConsoleWidget(QWidget* parent);
    ~ConsoleWidget();

    void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);

private:
    void keyPressEvent(QKeyEvent* e);

    QTextEdit* textField_;
    /// Log level colors
    QColor infoTextColor_;
    QColor warnTextColor_;
    QColor errorTextColor_;

public slots:
    void showContextMenu(const QPoint& pos);
};

} // namespace

#endif // IVW_CONSOLELISTWIDGET_H