#include <QVBoxLayout>

#include <inviwo/qt/editor/consolewidget.h>

namespace inviwo {

ConsoleWidget::ConsoleWidget(QWidget* parent) : InviwoDockWidget(tr("Console"), parent) {
    setObjectName("ConsoleWidget");
    setAllowedAreas(Qt::BottomDockWidgetArea);

    textField_ = new QTextEdit(this);
    textField_->setReadOnly(true);
    textField_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setWidget(textField_);

    LogCentral::instance()->registerLogger(this);
}

ConsoleWidget::~ConsoleWidget() {}

void ConsoleWidget::log(std::string logSource, unsigned int logLevel, const char* fileName,
                        const char* functionName, int lineNumber, std::string logMsg) {
    IVW_UNUSED_PARAM(functionName);
    if (logLevel == Error) {
        std::ostringstream lineNumberStr;
        lineNumberStr << lineNumber;
        textField_->setTextColor(Qt::red);
        textField_->append(QString::fromStdString(logSource + " (" + std::string(fileName) +
                           ", " + lineNumberStr.str() + "): " + logMsg));
        textField_->setTextColor(Qt::black);
    } else
        textField_->append(QString::fromStdString(logSource + ": " + logMsg));
}

} // namespace