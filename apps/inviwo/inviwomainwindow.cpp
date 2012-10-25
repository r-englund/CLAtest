
#include "inviwomainwindow.h"

#include "inviwo/qt/editor/consolewidget.h"
#include "inviwo/qt/editor/networkeditorview.h"
#include "inviwo/qt/editor/processorlistwidget.h"
#include "inviwo/qt/editor/propertylistwidget.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QList>
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

    addToolBars();
    addMenus();
    addMenuActions();

    rootDir_ = QString("D:/inviwo/data/");
    networkFileDir_ = rootDir_ + "workspaces/";

}

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

    //Dialog Window Setting
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebar_urls;
    sidebar_urls << QUrl::fromLocalFile(QDir(networkFileDir_).absolutePath());
    sidebar_urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebar_urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QFileDialog saveFileDialog(this, tr("Save Network ..."), QDir(networkFileDir_).absolutePath());
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setConfirmOverwrite(true);
    saveFileDialog.setNameFilters(extension);
    saveFileDialog.setSidebarUrls(sidebar_urls);

    if (saveFileDialog.exec()) {
        bool valid;
        QString path = saveFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv")) {
            valid = networkEditorView_->getNetworkEditor()->saveNetwork(path.toStdString() + ".inv");
        }
        else {
            valid = networkEditorView_->getNetworkEditor()->saveNetwork(path.toStdString());
        }
        networkFileDir_ = saveFileDialog.directory().path();

        return valid;
    }
    else {
        return false;
    }
    
}

bool InviwoMainWindow::loadNetwork() {

    //Dialog Window Setting
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebar_urls;
    sidebar_urls << QUrl::fromLocalFile(QDir(networkFileDir_).absolutePath());
    sidebar_urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebar_urls << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QFileDialog openFileDialog(this, tr("Open Network ..."), QDir(networkFileDir_).absolutePath());
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilters(extension);
    openFileDialog.setSidebarUrls(sidebar_urls);

    if (openFileDialog.exec()) {
        bool valid;
        QString path = openFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv")) {
            valid = networkEditorView_->getNetworkEditor()->loadNetwork(path.toStdString() + ".inv");
        }
        else {
            valid = networkEditorView_->getNetworkEditor()->loadNetwork(path.toStdString());
        }
        networkFileDir_ = openFileDialog.directory().path();
        return valid;
    }
    else {
        return false;
    }

}

void InviwoMainWindow::closeEvent(QCloseEvent *event) {
    networkEditorView_->getNetworkEditor()->clearNetwork();
}


InviwoMainWindow::~InviwoMainWindow() {}

} // namespace