#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include "inviwomainwindow.h"

#include <inviwo/qt/editor/networkeditorview.h>
#include <modules/openglqt/canvasqt.h>

#include <inviwo/core/network/processornetworkevaluator.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif
#include <QDesktopWidget>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

#include <inviwo/core/util/commandlineparser.h>

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() : VoidObserver() {
    NetworkEditor::init();

    // initialize console widget first to receive log messages
    consoleWidget_ = new ConsoleWidget(this);

    // the default render context managing the rendering state
    defaultRenderContext_ = new CanvasQt(this);

    currentWorkspaceFileName_ = "";
}

InviwoMainWindow::~InviwoMainWindow() {}

void InviwoMainWindow::initializeAndShow() {
    networkEditorView_ = new NetworkEditorView(this);
    setCentralWidget(networkEditorView_);

    mappingwidget_ = new MappingWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mappingwidget_);

    settingsWidget_ = new SettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, settingsWidget_);

    processorTreeWidget_ = new ProcessorTreeWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorTreeWidget_);

    propertyListWidget_ = new PropertyListWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, propertyListWidget_);
    addObservation(propertyListWidget_);
    propertyListWidget_->addObserver(this);

    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget_);

    // load settings and restore window state
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("state", saveState()).toByteArray());
    QPoint newPos = settings.value("pos", pos()).toPoint();
    QSize newSize = settings.value("size", size()).toSize();
    move(newPos);
    resize(newSize);
    /*
    QRect screenGeometry = QApplication::desktop()->screen()->geometry();
    screenGeometry.setRect(screenGeometry.x()-10, screenGeometry.y()-10,
                           screenGeometry.width()+20, screenGeometry.height()+20);
    if (!screenGeometry.contains(newPos) ||
        !screenGeometry.contains(QPoint(newPos.x()+newSize.width(), newPos.x()+newSize.height()))) {
            move(QPoint(0,0));
            resize(screenGeometry.width()-20, screenGeometry.height()-20);
    }
    */
    bool maximized = settings.value("maximized", true).toBool();
    recentFileList_ = settings.value("recentFileList").toStringList();
    lastExitWithoutErrors_ = settings.value("lastExitWithoutErrors", true).toBool();
    settings.setValue("lastExitWithoutErrors", false);
    settings.endGroup();

    rootDir_ = QString::fromStdString(IVW_DIR+"data/");
    workspaceFileDir_ = rootDir_ + "workspaces/";
    settingsWidget_->loadSettings();

    // initialize menus
    addMenus();
    addMenuActions();
    updateRecentWorkspaces();

    if (maximized) showMaximized();
    else show();
}

void InviwoMainWindow::deinitialize() {
}

void InviwoMainWindow::initializeWorkspace(){
    ProcessorNetworkEvaluator* networkEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
    networkEvaluator->setDefaultRenderContext(defaultRenderContext_);
    defaultRenderContext_->setFixedSize(0,0);
    defaultRenderContext_->initialize();
    defaultRenderContext_->activate();

    ProcessorNetwork* processorNetwork = const_cast<ProcessorNetwork*>(networkEditorView_->getNetworkEditor()->getProcessorNetwork());
    addObservation(processorNetwork);
    processorNetwork->addObserver(this);
}

void InviwoMainWindow::notify() {
    workspaceModified_ = true;
    updateWindowTitle();
}

bool InviwoMainWindow::processEndCommandLineArgs(){
    const CommandLineParser *cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();
    if (cmdparser->getCaptureAfterStartup()){
        ProcessorNetworkEvaluator* networkEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
        networkEvaluator->evaluate();
        std::string path = cmdparser->getOutputPath();
        if (path.empty())
            path = IVW_DIR+"data/images/";
        networkEvaluator->saveSnapshotAllCanvases(path, cmdparser->getSnapshotName());
    }

    if (cmdparser->getQuitApplicationAfterStartup())
        return false;

    return true;
}

void InviwoMainWindow::addMenus() {
    basicMenuBar = menuBar();
    fileMenuItem_ = basicMenuBar->addMenu(tr("&File"));
    viewMenuItem_ = basicMenuBar->addMenu(tr("&View"));
    helpMenuItem_ = basicMenuBar->addMenu(tr("&Help"));
}

void InviwoMainWindow::addMenuActions() {
    newFileAction_ = new QAction(QIcon(":/icons/new.png"), tr("&New Workspace"), this);
    newFileAction_->setShortcut(QKeySequence::New);
    connect(newFileAction_, SIGNAL(triggered()), this, SLOT(newWorkspace()));
    fileMenuItem_->addAction(newFileAction_);

    openFileAction_ = new QAction(QIcon(":/icons/open.png"), tr("&Open Workspace"), this);
    openFileAction_->setShortcut(QKeySequence::Open);
    connect(openFileAction_, SIGNAL(triggered()), this, SLOT(openWorkspace()));
    fileMenuItem_->addAction(openFileAction_);

    saveFileAction_ = new QAction(QIcon(":/icons/save.png"), tr("&Save Workspace"), this);
    saveFileAction_->setShortcut(QKeySequence::Save);
    connect(saveFileAction_, SIGNAL(triggered()), this, SLOT(saveWorkspace()));
    fileMenuItem_->addAction(saveFileAction_);

    saveAsFileAction_ = new QAction(QIcon(":/icons/saveas.png"), tr("&Save Workspace As"), this);
    connect(saveAsFileAction_, SIGNAL(triggered()), this, SLOT(saveWorkspaceAs()));
    fileMenuItem_->addAction(saveAsFileAction_);

    recentFileSeparator_ = fileMenuItem_->addSeparator();
    for (int i=0; i<maxNumRecentFiles_; i++) {
        recentFileActions_[i] = new QAction(this);
        recentFileActions_[i]->setVisible(false);
        connect(recentFileActions_[i], SIGNAL(triggered()), this, SLOT(openRecentWorkspace()));
        fileMenuItem_->addAction(recentFileActions_[i]);
    }
    
    mappingwidgetviewAction_ = new QAction(tr("&Mapping"), this);
    mappingwidgetviewAction_->setCheckable(true);
    mappingwidgetviewAction_->setChecked(mappingwidget_->isVisible());
    connect(mappingwidgetviewAction_, SIGNAL(triggered(bool)), mappingwidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(mappingwidgetviewAction_);

    settingsWidgetViewAction_ = new QAction(tr("&Settings"), this);
    settingsWidgetViewAction_->setCheckable(true);
    settingsWidgetViewAction_->setChecked(settingsWidget_->isVisible());
    connect(settingsWidgetViewAction_, SIGNAL(triggered(bool)), settingsWidget_, SLOT(setVisible(bool)));
    viewMenuItem_->addAction(settingsWidgetViewAction_);

    processorListWidgetViewAction_ = new QAction(tr("&Processor List"), this);
    processorListWidgetViewAction_->setCheckable(true);
    processorListWidgetViewAction_->setChecked(processorTreeWidget_->isVisible());
    connect(processorListWidgetViewAction_, SIGNAL(triggered(bool)), processorTreeWidget_, SLOT(setVisible(bool)));
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
    windowTitle.append(currentWorkspaceFileName_);
    if (workspaceModified_)
        windowTitle.append("*");
    setWindowTitle(windowTitle);
}

void InviwoMainWindow::updateRecentWorkspaces() {
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

void InviwoMainWindow::addToRecentWorkspaces(QString workspaceFileName) {
    recentFileList_.removeAll(workspaceFileName);
    recentFileList_.prepend(workspaceFileName);
    if (recentFileList_.size() > maxNumRecentFiles_)
        recentFileList_.removeLast();
    updateRecentWorkspaces();
}

void InviwoMainWindow::setCurrentWorkspace(QString workspaceFileName) {
    workspaceFileDir_ = QFileInfo(workspaceFileName).absolutePath();
    currentWorkspaceFileName_ = workspaceFileName;
    updateWindowTitle();
}

void InviwoMainWindow::newWorkspace() {
    if (currentWorkspaceFileName_ != "")
        askToSaveWorkspaceChanges();
    networkEditorView_->getNetworkEditor()->clearNetwork();
    setCurrentWorkspace(rootDir_ + "workspaces/untitled.inv");
    // set workspaceModified_ to true to get a * indicator in the window title
    workspaceModified_ = true;
    updateWindowTitle();
    // set it back to false to not ask to save an unmodified new workspace on exit
    workspaceModified_ = false;
}

void InviwoMainWindow::openWorkspace(QString workspaceFileName) {
    QFile file(workspaceFileName);
    if (!file.exists())
        return;
    
    networkEditorView_->getNetworkEditor()->loadNetwork(workspaceFileName.toLocal8Bit().constData());
    workspaceModified_ = false;
    setCurrentWorkspace(workspaceFileName);
    addToRecentWorkspaces(workspaceFileName);
}

void InviwoMainWindow::openLastWorkspace() {
    // if a workspace is defined by an argument, that workspace is opened, otherwise, the last opened workspace is used
    const CommandLineParser *cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();
    if (cmdparser->getLoadWorkspaceFromArg())
        openWorkspace(static_cast<const QString>(cmdparser->getWorkspacePath().c_str()));
    else if (!recentFileList_.isEmpty() && lastExitWithoutErrors_)
        openWorkspace(recentFileList_[0]);
    else
        newWorkspace();
}

void InviwoMainWindow::openWorkspace() {
    askToSaveWorkspaceChanges();

    // dialog window settings
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(workspaceFileDir_).absolutePath());
    //TODO: create InviwoFileDialog to avoid frequent version checks
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif

    QFileDialog openFileDialog(this, tr("Open Workspace ..."), QDir(workspaceFileDir_).absolutePath());
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilters(extension);
    openFileDialog.setSidebarUrls(sidebarURLs);

    if (openFileDialog.exec()) {
        QString path = openFileDialog.selectedFiles().at(0);
        openWorkspace(path);
    }
}

void InviwoMainWindow::openRecentWorkspace() {
    QAction* action = qobject_cast<QAction*>(sender());
    if (action) {
        askToSaveWorkspaceChanges();
        openWorkspace(action->data().toString());
    }
}

void InviwoMainWindow::saveWorkspace() {
    if (currentWorkspaceFileName_.contains("untitled.inv")) saveWorkspaceAs();
    else {
        networkEditorView_->getNetworkEditor()->saveNetwork(currentWorkspaceFileName_.toLocal8Bit().constData());
        workspaceModified_ = false;
        updateWindowTitle();
    }
    /*
    // FIXME: the following code snippet allows to reload the Qt style sheets during runtime,
    // which is handy while we change them. once the style hseets have been finalized,
    // this code should be removed.
    QFile styleSheetFile("D:/inviwo/resources/stylesheets/inviwo.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr())->setStyleSheet(styleSheet);
    styleSheetFile.close();
    */
}

void InviwoMainWindow::saveWorkspaceAs() {
    // dialog window settings
    QStringList extension;
    extension << "Inviwo File (*.inv)";

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(workspaceFileDir_).absolutePath());
    //TODO: create InviwoFileDialog to avoid frequent version checks
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif

    QFileDialog saveFileDialog(this, tr("Save Workspace ..."), QDir(workspaceFileDir_).absolutePath());
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDialog.setConfirmOverwrite(true);
    saveFileDialog.setNameFilters(extension);
    saveFileDialog.setSidebarUrls(sidebarURLs);

    if (saveFileDialog.exec()) {
        QString path = saveFileDialog.selectedFiles().at(0);
        if (!path.endsWith(".inv")) path.append(".inv");
        networkEditorView_->getNetworkEditor()->saveNetwork(path.toLocal8Bit().constData());
        workspaceModified_ = false;
        setCurrentWorkspace(path);
        addToRecentWorkspaces(path);
    }
}

void InviwoMainWindow::closeEvent(QCloseEvent* event) {
    IVW_UNUSED_PARAM(event);

    if (!askToSaveWorkspaceChanges()) {
        event->ignore();
        return;
    }
    networkEditorView_->getNetworkEditor()->clearNetwork();

    // save window state
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("mainwindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());
    settings.setValue("maximized", isMaximized());
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("recentFileList", recentFileList_);
    settings.setValue("lastExitWithoutErrors", true);
    settings.endGroup();

    QMainWindow::closeEvent(event);
}

bool InviwoMainWindow::askToSaveWorkspaceChanges() {
    bool continueOperation = true;
    if (workspaceModified_) {
        QMessageBox msgBox;
        msgBox.setText("Workspace Modified");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int answer = msgBox.exec();
        switch (answer) {
                case QMessageBox::Yes:
                    saveWorkspace();
                    break;
                case QMessageBox::No:
                    break;
                case QMessageBox::Cancel:
                    continueOperation = false;
                    break;
        }
    }
    return continueOperation;
}

} // namespace