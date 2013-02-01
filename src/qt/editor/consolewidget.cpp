#include <QVBoxLayout>

#include <inviwo/qt/editor/consolewidget.h>

namespace inviwo {

ConsoleWidget::ConsoleWidget(QWidget* parent) : InviwoDockWidget(tr("Console"), parent)
, infoTextColor_(233,233, 233), warnTextColor_(221,165,8), errorTextColor_(255,107,107) {
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
    QString message;
    switch (logLevel) {
        case Info: 
            textField_->setTextColor(infoTextColor_);
            message = QString::fromStdString(logSource + ": " + logMsg);
            break;
        case Warn: 
            textField_->setTextColor(warnTextColor_);
            message = QString::fromStdString(logSource + ": " + logMsg);
            break;
        case Error: {
            textField_->setTextColor(errorTextColor_);
            std::ostringstream lineNumberStr;
            lineNumberStr << lineNumber;
            message = QString::fromStdString(logSource + " (" + std::string(fileName) +
                ", " + lineNumberStr.str() + "): " + logMsg);
            break;
        }
        default:
            textField_->setTextColor(infoTextColor_);
            message = QString::fromStdString(logSource + ": " + logMsg);
            //textField_->append(QString::fromStdString(logSource + ": " + logMsg)); 
    }
    textField_->append(message); 

}

} // namespace