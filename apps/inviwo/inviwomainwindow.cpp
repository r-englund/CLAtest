
#include "inviwomainwindow.h"

#include "inviwo/qt/editor/networkeditorview.h"
#include "inviwo/qt/editor/processorlistwidget.h"

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() {
    NetworkEditorView* networkEditorView = new NetworkEditorView(this);
    setCentralWidget(networkEditorView);

    ProcessorListWidget* processorListWidget = new ProcessorListWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorListWidget);
}

InviwoMainWindow::~InviwoMainWindow() {}

} // namespace