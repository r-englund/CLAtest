#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include "inviwomainwindow.h"

#include <inviwo/qt/editor/networkeditorview.h>
#include <inviwo/qt/widgets/canvasqt.h>

#include <inviwo/core/network/processornetworkevaluator.h>

#include <QDesktopServices>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() {
    NetworkEditor::init();
    networkEditorView_ = new NetworkEditorView(this);
    setCentralWidget(networkEditorView_);
    defaultRenderContext_ = new CanvasQt(this);    
    defaultRenderContext_->switchContext();

    consoleWidget_ = new ConsoleWidget(this);

    addMenus();
}

InviwoMainWindow::~InviwoMainWindow() {}

void InviwoMainWindow::initialize() {
    networkEditorView_ = new NetworkEditorView(this);
    setCentralWidget(networkEditorView_);

    settingsWidget_ = new SettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, settingsWidget_);

    processorListWidget_ = new ProcessorListWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorListWidget_);

    propertyListWidget_ = new PropertyListWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, propertyListWidget_);

    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget_);

    // load settings and restore window state
    QSettings settings("Inviwo", "Inviwo");
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
    rootDir_ = QString::fromStdString(IVW_DIR+"data/");
    networkFileDir_ = rootDir_ + "workspaces/";
    recentFileList_ = settings.value("recentFileList").toStringList();
    settingsWidget_->loadSettings();

    initialized_ = true;
}

void InviwoMainWindow::deinitialize() {
    initialized_ = false;
}

void InviwoMainWindow::setupEnvironment(){
    addMenuActions();

    ProcessorNetworkEvaluator* processEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
    processEvaluator->setDefaultRenderContext(defaultRenderContext_);
    defaultRenderContext_->setFixedSize(0,0);

    // restore previous files
    updateRecentNetworks();
    newNetwork();
}

void InviwoMainWindow::addMenus() {
    basicMenuBar = menuBar();
    fileMenuItem_ = basicMenuBar->addMenu(tr("&File"));
    viewMenuItem_ = basicMenuBar->addMenu(tr("&View"));
    helpMenuItem_ = basicMenuBar->addMenu(tr("&Help"));
}

void InviwoMainWindow::addMenuActions() {
    newFileAction_ = new QAction(QIcon(":/icons/network_new.png"), tr("&New Network"), this);
    newFileAction_->setShortcut(QKeySequence::New);
    connect(newFileAction_, SIGNAL(triggered()), this, SLOT(newNetwork()));
    fileMenuItem_->addAction(newFileAction_);

    openFileAction_ = new QAction(QIcon(":/icons/network_open.png"), tr("&Open Network"), this);
    openFileAction_->setShortcut(QKeySequence::Open);
    connect(openFileAction_, SIGNAL(triggered()), this, SLOT(openNetwork()));
    fileMenuItem_->addAction(openFileAction_);

    saveFileAction_ = new QAction(QIcon(":/icons/network_save.png"), tr("&Save Network"), this);
    saveFileAction_->setShortcut(QKeySequence::Save);
    connect(saveFileAction_, SIGNAL(triggered()), this, SLOT(saveNetwork()));
    fileMenuItem_->addAction(saveFileAction_);

    saveAsFileAction_ = new QAction(QIcon(":/icons/network_saveas.png"), tr("&Save Network As"), this);
    connect(saveAsFileAction_, SIGNAL(triggered()), this, SLOT(saveNetworkAs()));
    fileMenuItem_->addAction(saveAsFileAction_);

    recentFileSeparator_ = fileMenuItem_->addSeparator();
    for (int i=0; i<maxNumRecentFiles_; i++) {
        recentFileActions_[i] = new QAction(this);
        recentFileActions_[i]->setVisible(false);
        connect(recentFileActions_[i], SIGNAL(triggered()), this, SLOT(openRecentNetwork()));
        fileMenuItem_->addAction(recentFileActions_[i]);
    }

    settingsWidgetViewAction_ = new QAction(tr("&Settings"), this);
    settingsWidgetViewAction_->setCheckable(true);
    settingsWidgetViewAction_->setChecked(settingsWidget_->isVisible());
    connect(settingsWidgetViewAction_, SIGNAL(triggered(bool)), settingsWidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(settingsWidgetViewAction_);

    processorListWidgetViewAction_ = new QAction(tr("&Processor List"), this);
    processorListWidgetViewAction_->setCheckable(true);
    processorListWidgetViewAction_->setChecked(processorListWidget_->isVisible());
    connect(processorListWidgetViewAction_, SIGNAL(triggered(bool)), processorListWidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(processorListWidgetViewAction_);

    propertyListWidgetViewAction_ = new QAction(tr("&Property List"), this);
    propertyListWidgetViewAction_->setCheckable(true);
    propertyListWidgetViewAction_->setChecked(propertyListWidget_->isVisible());
    connect(propertyListWidgetViewAction_, SIGNAL(triggered(bool)), propertyListWidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(propertyListWidgetViewAction_);

    consoleWidgetViewAction_ = new QAction(tr("&Output Console"), this);
    consoleWidgetViewAction_->setCheckable(true);
    consoleWidgetViewAction_->setChecked(consoleWidget_->isVisible());
    connect(consoleWidgetViewAction_, SIGNAL(triggered(bool)), consoleWidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(consoleWidgetViewAction_);
}

void InviwoMainWindow::updateWindowTitle() {
    QString windowTitle = QString("Inviwo - Interactive Visualization Workshop - ");
    windowTitle.append(currentNetworkFileName_);
    if (networkEditorView_->getNetworkEditor()->getProcessorNetwork()->isModified())
        windowTitle.append("*");
    setWindowTitle(windowTitle);
}

void InviwoMainWindow::updateRecentNetworks() {
    for (int i=0; i<recentFileList_.size(); i++) {
        if (!recentFileList_[i].isEmpty()) {
            QString menuEntry = tr("&%1 %2").arg(i + 1).arg(QFileInfo(recentFileList_[i]).fileName());
            recentFileActions_[i]->setText(menuEntry);
            recentFileActions_[i]->setData(recentFileList_[i]);
            recentFileActions_[i]->setVisible(true);
        } else recentFileActions_[i]->setVisible(false);
    }
    recentFileSeparator_->setVisible(recentFileList_.size() > 0);
}

void InviwoMainWindow::addToRecentNetworks(QString networkFileName) {
    recentFileList_.removeAll(networkFileName);
    recentFileList_.prepend(networkFileName);
    if (recentFileList_.size() > maxNumRecentFiles_)
        recentFileList_.removeLast();
    updateRecentNetworks();
}

void InviwoMainWindow::setCurrentNetwork(QString networkFileName) {
    networkFileDir_ = QFileInfo(networkFileName).absolutePath();
    currentNetworkFileName_ = networkFileName;
    updateWindowTitle();
}

void InviwoMainWindow::newNetwork() {
    networkEditorView_->getNetworkEditor()->clearNetwork();
    setCurrentNetwork(rootDir_ + "workspaces/untitled.inv");
}

void InviwoMainWindow::openNetwork(QString networkFileName) {
    QFile file(networkFileName);
    if(!file.exists())
        return;
    
    networkEditorView_->getNetworkEditor()->loadNetwork(networkFileName.toStdString());
    setCurrentNetwork(networkFileName);
    addToRecentNetworks(networkFileName);
}

void InviwoMainWindow::openLastNetwork() {
    if (!recentFileList_.isEmpty())
        openNetwork(recentFileList_[0]);
}

void InviwoMainWindow::openNetwork() {
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
        QString path = openFileDialog.selectedFiles().at(0);
        openNetwork(path);
    }
}

void InviwoMainWindow::openRecentNetwork() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action)
        openNetwork(action->data().toString());
}

void InviwoMainWindow::saveNetwork() {
    networkEditorView_->getNetworkEditor()->saveNetwork(currentNetworkFileName_.toStdString());
    updateWindowTitle();
}

void InviwoMainWindow::saveNetworkAs() {
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
        QString path = saveFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv")) path.append(".inv");
        networkEditorView_->getNetworkEditor()->saveNetwork(path.toStdString());
        setCurrentNetwork(path);
        addToRecentNetworks(path);
    }
}

void InviwoMainWindow::closeEvent(QCloseEvent* event) {
    IVW_UNUSED_PARAM(event);
    networkEditorView_->getNetworkEditor()->clearNetwork();
    // save window state
    QSettings settings("Inviwo","Inviwo");
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.setValue("recentFileList", recentFileList_);

    QMainWindow::closeEvent(event);
}

} // namespace