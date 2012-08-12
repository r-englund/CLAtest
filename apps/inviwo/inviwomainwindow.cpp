
#include "inviwomainwindow.h"

#include "inviwo/qt/editor/networkeditorview.h"
#include "inviwo/qt/editor/processorlistwidget.h"

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() {
    NetworkEditorView* dataFlowEditorView = new NetworkEditorView(this);
    setCentralWidget(dataFlowEditorView);

    ProcessorListWidget* processorListWidget = new ProcessorListWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorListWidget);

    //connect( &CWrapperCanvas::instance(), SIGNAL(wrapperSelected(QObject*)),
    //       d->ui.propertyEditor, SLOT(setObject(QObject*)));
}

InviwoMainWindow::~InviwoMainWindow() {}

} // namespace