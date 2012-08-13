#include <QTextEdit>
#include <QVBoxLayout>

#include "inviwo/qt/editor/consolewidget.h"

namespace inviwo {

ConsoleWidget::ConsoleWidget(QWidget* parent) : QDockWidget(tr("Console"), parent) {
    setAllowedAreas(Qt::BottomDockWidgetArea);

    QTextEdit* textField = new QTextEdit(this);
    textField->setReadOnly(true);
    textField->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    /*
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(textField);
    setLayout(vBoxLayout);
    */
    setWidget(textField);
}

ConsoleWidget::~ConsoleWidget() {}

} // namespace