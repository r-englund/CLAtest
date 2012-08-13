
#include "inviwomainwindow.h"

#include "inviwo/qt/editor/consolewidget.h"
#include "inviwo/qt/editor/networkeditorview.h"
#include "inviwo/qt/editor/processorlistwidget.h"
#include "inviwo/qt/editor/propertylistwidget.h"

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() {
    NetworkEditorView* networkEditorView = new NetworkEditorView(this);
    setCentralWidget(networkEditorView);

    ProcessorListWidget* processorListWidget = new ProcessorListWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorListWidget);

    PropertyListWidget* propertyListWidget = new PropertyListWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, propertyListWidget);

    ConsoleWidget* consoleWidget = new ConsoleWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget);
}

InviwoMainWindow::~InviwoMainWindow() {}

} // namespace