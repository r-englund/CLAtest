
#include "inviwomainwindow.h"

#include "inviwo/qt/editor/consolewidget.h"
#include "inviwo/qt/editor/networkeditorview.h"
#include "inviwo/qt/editor/processorlistwidget.h"
#include "inviwo/qt/editor/propertylistwidget.h"
#include "inviwo/qt/widgets/canvasqt.h"

#include "inviwo/core/network/processornetworkevaluator.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() {
    networkEditorView_ = new NetworkEditorView(this);
    setCentralWidget(networkEditorView_);

    ProcessorListWidget* processorListWidget = new ProcessorListWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorListWidget);

    PropertyListWidget* propertyListWidget = new PropertyListWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, propertyListWidget);

    ConsoleWidget* consoleWidget = new ConsoleWidget(this);
    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget);

    defaultRenderContext_ = new CanvasQt(this);    
    defaultRenderContext_->switchContext();
    ProcessorNetworkEvaluator* processEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
    processEvaluator->setDefaultRenderContext(defaultRenderContext_);
    defaultRenderContext_->setFixedSize(0,0);

    addToolBars();
    addMenus();
    addMenuActions();

    setWindowTitle("Inviwo - Interactive Visualization Workshop");

    rootDir_ = QString::fromStdString(IVW_DIR+"data/");
    networkFileDir_ = rootDir_ + "workspaces/";

    // restore window state
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}

InviwoMainWindow::~InviwoMainWindow() {}

void InviwoMainWindow::addToolBars() {
    //basicToolbar_ = addToolBar(tr("File"));
}


void InviwoMainWindow::addMenus() {
    basicMenuBar = menuBar();
    fileMenuItem_ = basicMenuBar->addMenu(tr("&File"));
    editMenuItem_ = basicMenuBar->addMenu(tr("&Edit"));
}

void InviwoMainWindow::addMenuActions() {
    openFileAction_ = new QAction(tr("&Open"),  this);
    connect(openFileAction_, SIGNAL(triggered()), this, SLOT(loadNetwork()));
    fileMenuItem_->addAction(openFileAction_);

    // NetworkEditors File Save
    saveFileAction_ = new QAction(tr("&Save"),  this);
    connect(saveFileAction_, SIGNAL(triggered()), this, SLOT(saveNetwork()));
    fileMenuItem_->addAction(saveFileAction_);
}


bool InviwoMainWindow::saveNetwork() {
    // dialog window settings
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(networkFileDir_).absolutePath());
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QFileDialog saveFileDialog(this, tr("Save Network ..."), QDir(networkFileDir_).absolutePath());
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setConfirmOverwrite(true);
    saveFileDialog.setNameFilters(extension);
    saveFileDialog.setSidebarUrls(sidebarURLs);

    if (saveFileDialog.exec()) {
        bool valid;
        QString path = saveFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv"))
            valid = networkEditorView_->getNetworkEditor()->saveNetwork(path.toStdString() + ".inv");
        else
            valid = networkEditorView_->getNetworkEditor()->saveNetwork(path.toStdString());
        networkFileDir_ = saveFileDialog.directory().path();
        return valid;
    } else
        return false;
}

bool InviwoMainWindow::loadNetwork() {
    // dialog window settings
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(networkFileDir_).absolutePath());
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QFileDialog openFileDialog(this, tr("Open Network ..."), QDir(networkFileDir_).absolutePath());
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilters(extension);
    openFileDialog.setSidebarUrls(sidebarURLs);

    if (openFileDialog.exec()) {
        bool valid;
        QString path = openFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv"))
            valid = networkEditorView_->getNetworkEditor()->loadNetwork(path.toStdString() + ".inv");
        else
            valid = networkEditorView_->getNetworkEditor()->loadNetwork(path.toStdString());
        networkFileDir_ = openFileDialog.directory().path();
        return valid;
    } else
        return false;
}

void InviwoMainWindow::closeEvent(QCloseEvent* event) {
    IVW_UNUSED_PARAM(event);
    networkEditorView_->getNetworkEditor()->clearNetwork();
    // save window state
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    QMainWindow::closeEvent(event);
}

} // namespace