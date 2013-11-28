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
#include <QActionGroup>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

#include <inviwo/core/util/commandlineparser.h>

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() : QMainWindow(), VoidObserver() {
    NetworkEditor::init();

    // initialize console widget first to receive log messages
    consoleWidget_ = new ConsoleWidget(this);

    // the default render context managing the rendering state
    defaultRenderContext_ = new CanvasQt(this);

    currentWorkspaceFileName_ = "";
}

InviwoMainWindow::~InviwoMainWindow() {
	deinitialize();
}

void InviwoMainWindow::initialize() {
    networkEditorView_ = new NetworkEditorView(this);
    setCentralWidget(networkEditorView_);

    resourceManagerWidget_ = new ResourceManagerWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, resourceManagerWidget_);
	resourceManagerWidget_->hide();

    mappingwidget_ = new MappingWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, mappingwidget_);
	mappingwidget_->hide();

    settingsWidget_ = new SettingsWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, settingsWidget_);
	settingsWidget_->hide();

    processorTreeWidget_ = new ProcessorTreeWidget(this);
    addDockWidget(Qt::LeftDockWidgetArea, processorTreeWidget_);

    propertyListWidget_ = new PropertyListWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, propertyListWidget_);
    addObservation(propertyListWidget_);
    propertyListWidget_->addObserver(this);

    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget_);

#ifdef IVW_HAS_PYTHON
	pythonEditorWidget_ = new PythonEditorWidget(this);
    InviwoApplication::getRef().registerFileObserver(pythonEditorWidget_);
#endif

    // load settings and restore window state
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry", saveGeometry()).toByteArray());
    restoreState(settings.value("state", saveState()).toByteArray());
    QPoint newPos = settings.value("pos", pos()).toPoint();
    QSize newSize = settings.value("size", size()).toSize();

	maximized_ = settings.value("maximized", true).toBool();
    QDesktopWidget* desktop = QApplication::desktop();
	QRect wholeScreenGeometry = desktop->screenGeometry(0);
	for (int i=1; i<desktop->screenCount(); i++)
		wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));
	wholeScreenGeometry.setRect(wholeScreenGeometry.x()-10, wholeScreenGeometry.y()-10,
		wholeScreenGeometry.width()+20, wholeScreenGeometry.height()+20);

	QPoint bottomRight = QPoint(newPos.x()+newSize.width(), newPos.y()+newSize.height());
	if (!wholeScreenGeometry.contains(newPos) || !wholeScreenGeometry.contains(bottomRight)) {
		move(QPoint(0,0));
		resize(wholeScreenGeometry.width()-20, wholeScreenGeometry.height()-20);
	} else {
		move(newPos);
		resize(newSize);
	}

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
	addToolBars();
    updateRecentWorkspaces();
}
void InviwoMainWindow::showWindow(){
    if (maximized_) showMaximized();
    else show();
};
        
void InviwoMainWindow::deinitialize() {
}

void InviwoMainWindow::initializeWorkspace(){
    ProcessorNetworkEvaluator* networkEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
    networkEvaluator->setDefaultRenderContext(defaultRenderContext_);
    defaultRenderContext_->setFixedSize(0,0);
    defaultRenderContext_->initializeSquare();
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
    const CommandLineParser* cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();

#ifdef IVW_HAS_PYTHON
    if (cmdparser->getRunPythonScriptAfterStartup()) {
        pythonEditorWidget_->show();
        pythonEditorWidget_->loadFile(cmdparser->getPythonScirptName(),false);
        pythonEditorWidget_->run();
    }
#endif
	if (cmdparser->getCaptureAfterStartup()) {
		ProcessorNetworkEvaluator* networkEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
		networkEvaluator->evaluate();
		std::string path = cmdparser->getOutputPath();
		if (path.empty())
			path = IVW_DIR+"data/images/";
		networkEvaluator->saveSnapshotAllCanvases(path, cmdparser->getSnapshotName());
	}

	if (cmdparser->getScreenGrabAfterStartup()) {
		std::string path = cmdparser->getOutputPath();
		if (path.empty())
			path = IVW_DIR+"data/images/";
		repaint();
		int curScreen = QApplication::desktop()->screenNumber(this);
		QPixmap screenGrab = QPixmap::grabWindow(QApplication::desktop()->screen(curScreen)->winId());
		//QPixmap screenGrab = QPixmap::grabWindow(winId());
		std::string fileName = cmdparser->getScreenGrabName();
		screenGrab.save(QString::fromStdString(path + "/" + fileName), "png");
	}

    if (cmdparser->getQuitApplicationAfterStartup())
        return false;

    return true;
}

std::vector<std::string> InviwoMainWindow::getWorkspaceSnapshots(std::string workspaceFileName) {
    return networkEditorView_->getNetworkEditor()->getSnapshotsOfExternalNetwork(workspaceFileName);
}

void InviwoMainWindow::addMenus() {
	basicMenuBar = menuBar();

	QAction* first = 0;
	if(basicMenuBar->actions().size()>0)
		first = basicMenuBar->actions()[0];

	fileMenuItem_ = new QMenu(tr("&File"));
	viewMenuItem_ = new QMenu(tr("&View"));
	viewModeItem_ = new QMenu(tr("&View mode"));
	basicMenuBar->insertMenu(first, fileMenuItem_);
	basicMenuBar->insertMenu(first, viewMenuItem_);
	viewMenuItem_->addMenu(viewModeItem_);

	helpMenuItem_ = basicMenuBar->addMenu(tr("&Help"));

#ifdef IVW_HAS_PYTHON
    pythonMenuItem_ = new QMenu(tr("&Python"));
    basicMenuBar->insertMenu(first, pythonMenuItem_);
#endif
}

void InviwoMainWindow::addMenuActions() {
	// file menu entries
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
    

	// dockwidget visibility menu entries
    viewMenuItem_->addAction(mappingwidget_->toggleViewAction());
    viewMenuItem_->addAction(settingsWidget_->toggleViewAction());
    processorTreeWidget_->toggleViewAction()->setText(tr("&Processor List"));
    viewMenuItem_->addAction(processorTreeWidget_->toggleViewAction());
    propertyListWidget_->toggleViewAction()->setText(tr("&Property List"));
    viewMenuItem_->addAction(propertyListWidget_->toggleViewAction());
    consoleWidget_->toggleViewAction()->setText(tr("&Output Console"));
	viewMenuItem_->addAction(consoleWidget_->toggleViewAction());
	viewMenuItem_->addAction(resourceManagerWidget_->toggleViewAction());


	// application/developer mode menu entries
	developerViewModeAction_ = new QAction(tr("&Developer"),this);
	developerViewModeAction_->setCheckable(true);
	developerViewModeAction_->setIcon(QIcon(":/icons/view-developer.png"));
	viewModeItem_->addAction(developerViewModeAction_);

	applicationViewModeAction_ = new QAction(tr("&Application"),this);
	applicationViewModeAction_->setCheckable(true);
	applicationViewModeAction_->setIcon(QIcon(":/icons/view-application.png"));
	viewModeItem_->addAction(applicationViewModeAction_);

	QActionGroup* actionGroup = new QActionGroup(this);
	actionGroup->addAction(developerViewModeAction_);
	actionGroup->addAction(applicationViewModeAction_);

	PropertyVisibilityMode visibilityMode = propertyListWidget_->getVisibilityMode();
	if (visibilityMode == DEVELOPMENT){
		developerViewModeAction_->setChecked(true);
	}
	if (visibilityMode == APPLICATION) {
		applicationViewModeAction_->setChecked(true);
	}

	connect(developerViewModeAction_, SIGNAL(triggered(bool)), propertyListWidget_, SLOT(setDeveloperViewMode(bool)));
	connect(applicationViewModeAction_, SIGNAL(triggered(bool)), propertyListWidget_, SLOT(setApplicationViewMode(bool))); 

#ifdef IVW_HAS_PYTHON
    pythonEditorOpenAction_ = new QAction(QIcon(":/icons/python.png"),"&Python Editor", this);
    connect(pythonEditorOpenAction_, SIGNAL(triggered(bool)), this, SLOT(show(void)));
    pythonMenuItem_->addAction(pythonEditorOpenAction_);
#endif
}

void InviwoMainWindow::addToolBars() {
	fileToolBar_ = addToolBar("File");
	fileToolBar_->setObjectName("fileToolBar");
	fileToolBar_->addAction(newFileAction_);
	fileToolBar_->addAction(openFileAction_);
	fileToolBar_->addAction(saveFileAction_);
	fileToolBar_->addAction(saveAsFileAction_);

	viewToolBar_ = addToolBar("View");
	viewToolBar_->setObjectName("viewToolBar");
	viewToolBar_->addAction(developerViewModeAction_);
	viewToolBar_->addAction(applicationViewModeAction_);
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

std::string InviwoMainWindow::getCurrentWorkspace() {
    return currentWorkspaceFileName_.toLocal8Bit().constData();
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
    // which is handy while we change them. once the style sheets have been finalized,
    // this code should be removed.
    QFile styleSheetFile("C:/inviwo/resources/stylesheets/inviwo.qss");
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

    settingsWidget_->saveSettings();
    propertyListWidget_->saveState();

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