#include <QTextEdit>
#include <QVBoxLayout>

#include "inviwo/qt/editor/consolewidget.h"

namespace inviwo {

ConsoleWidget::ConsoleWidget(QWidget* parent) : InviwoDockWidget(tr("Console"), parent) {
    setAllowedAreas(Qt::BottomDockWidgetArea);

    QTextEdit* textField = new QTextEdit(this);
    textField->setReadOnly(true);
    textField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    setWidget(textField);
}

ConsoleWidget::~ConsoleWidget() {}

} // namespace