 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include "inviwomainwindow.h"
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/editor/networkeditorview.h>
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


#ifdef IVW_PYTHON_QT
#include <modules/pythonqt/pythoneditorwidget.h>
#endif

namespace inviwo { 

InviwoMainWindow::InviwoMainWindow() : QMainWindow(), VoidObserver() {
    NetworkEditor::init();

    // initialize console widget first to receive log messages
    consoleWidget_ = new ConsoleWidget(this);

    currentWorkspaceFileName_ = "";
}

InviwoMainWindow::~InviwoMainWindow() {
	deinitialize();
    NetworkEditor::deleteInstance();
}

void InviwoMainWindow::initialize() {
    networkEditorView_ = new NetworkEditorView(this);
    NetworkEditorObserver::addObservation(networkEditorView_->getNetworkEditor());
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
    VoidObserver::addObservation(propertyListWidget_);
    propertyListWidget_->addObserver(this);

    addDockWidget(Qt::BottomDockWidgetArea, consoleWidget_);

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

    rootDir_ = QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_DATA));
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
    ProcessorNetwork* processorNetwork = const_cast<ProcessorNetwork*>(networkEditorView_->getNetworkEditor()->getProcessorNetwork());
    VoidObserver::addObservation(processorNetwork);
    processorNetwork->addObserver(this);
}

void InviwoMainWindow::notify() {
    workspaceModified_ = true;
    updateWindowTitle();
}

bool InviwoMainWindow::processEndCommandLineArgs(){
    const CommandLineParser* cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();

#ifdef IVW_PYTHON_QT
    if (cmdparser->getRunPythonScriptAfterStartup()) {
        PythonEditorWidget::getPtr()->show();
        PythonEditorWidget::getPtr()->loadFile(cmdparser->getPythonScirptName(),false);
        PythonEditorWidget::getPtr()->run();
    }
#endif
	if (cmdparser->getCaptureAfterStartup()) {
		ProcessorNetworkEvaluator* networkEvaluator = networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator();
		networkEvaluator->requestEvaluate();
		std::string path = cmdparser->getOutputPath();
		if (path.empty())
			path = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES);
		networkEvaluator->saveSnapshotAllCanvases(path, cmdparser->getSnapshotName());
	}

	if (cmdparser->getScreenGrabAfterStartup()) {
		std::string path = cmdparser->getOutputPath();
		if (path.empty())
			path = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES);
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

void InviwoMainWindow::addMenus() {
	basicMenuBar = menuBar();

	QAction* first = 0;
	if(basicMenuBar->actions().size()>0)
		first = basicMenuBar->actions()[0];

	fileMenuItem_ = new QMenu(tr("&File"),basicMenuBar);
	viewMenuItem_ = new QMenu(tr("&View"),basicMenuBar);
    viewModeItem_ = new QMenu(tr("&View mode"),basicMenuBar);
    helpMenuItem_ = new QMenu(tr("&Help"),basicMenuBar);
	basicMenuBar->insertMenu(first, fileMenuItem_);
	basicMenuBar->insertMenu(first, viewMenuItem_);
	viewMenuItem_->addMenu(viewModeItem_);
    basicMenuBar->insertMenu(first, helpMenuItem_);
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

    recentFileSeparator_ = fileMenuItem_->addSeparator();
    exitAction_ = new QAction(tr("&Exit"), this);
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(exitInviwo()));
    fileMenuItem_->addAction(exitAction_);

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

    enableDisableEvaluationButton_ = new QToolButton(this);
    enableDisableEvaluationButton_->setToolTip(tr("Enable/Disable Evaluation"));
    enableDisableEvaluationButton_->setCheckable(true);
    enableDisableEvaluationButton_->setChecked(false);
    QIcon enableDisableIcon;
    enableDisableIcon.addFile(":/icons/button_ok.png", QSize(), QIcon::Active, QIcon::Off);
    enableDisableIcon.addFile(":/icons/button_cancel.png", QSize(), QIcon::Active, QIcon::On);
    enableDisableEvaluationButton_->setIcon(enableDisableIcon);
    connect(enableDisableEvaluationButton_, SIGNAL(toggled(bool)), this, SLOT(disableEvaluation(bool)));

    aboutBoxAction_ = new QAction(QIcon(":/icons/about.png"), tr("&About"), this);
    connect(aboutBoxAction_, SIGNAL(triggered()), this, SLOT(showAboutBox()));
    helpMenuItem_->addAction(aboutBoxAction_);
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
    viewToolBar_->addWidget(enableDisableEvaluationButton_);
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
#ifdef IVW_PYTHON_QT
    if(PythonEditorWidget::getPtr()->isActiveWindow() && PythonEditorWidget::getPtr()->hasFocus()){
        PythonEditorWidget::getPtr()->setDefaultText();
        return;
    }
#endif
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
    if(!file.exists()) {
        LogError("Could not find workspace file: " << workspaceFileName.toLocal8Bit().constData());
        return;
    }
    
    networkEditorView_->getNetworkEditor()->loadNetwork(workspaceFileName.toLocal8Bit().constData());
    workspaceModified_ = false;
    onNetworkEditorFileChanged(workspaceFileName.toLocal8Bit().constData());
}

void InviwoMainWindow::onNetworkEditorFileChanged(const std::string &filename){
    setCurrentWorkspace(filename.c_str());
    addToRecentWorkspaces(filename.c_str());
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

#ifdef IVW_PYTHON_QT
    if(PythonEditorWidget::getPtr()->isActiveWindow() && PythonEditorWidget::getPtr()->hasFocus()){
        PythonEditorWidget::getPtr()->save();
        return;
    } // only save workspace if ptyhon editor does not have focus
#endif

    if (currentWorkspaceFileName_.contains("untitled.inv")) saveWorkspaceAs();
    else {
        networkEditorView_->getNetworkEditor()->saveNetwork(currentWorkspaceFileName_.toLocal8Bit().constData());
        workspaceModified_ = false;
        updateWindowTitle();
    }

    /*
    // The following code snippet allows to reload the Qt style sheets during runtime,
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

void InviwoMainWindow::disableEvaluation(bool disable){
    if(disable)
        networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator()->disableEvaluation();
    else
        networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator()->enableEvaluation();
}

void InviwoMainWindow::showAboutBox() {
    std::string aboutText;
    aboutText.append("<b>Inviwo V"+IVW_VERSION+"</b><br>");
    aboutText.append("Interactive Visualization Workshop<br>");
    aboutText.append("(C) 2012-2014 The Inviwo Foundation<br>");
    aboutText.append("<a href='http://www.inviwo.org/'>http://www.inviwo.org/</a>");
    aboutText.append("<p>Inviwo is a rapid prototyping environment for interactive \
                     visualizations. It is licensed under the Simplified BSD license.</p>");
    aboutText.append("<p><b>Core Team:</b><br>");
    aboutText.append("Rickard Englund, Daniel Jönsson, Sathish Kottravel, Timo Ropinski, Peter Steneteg, Erik Sundén</p>");
    QMessageBox::about(this, QString::fromStdString("Inviwo V"+IVW_VERSION), QString::fromStdString(aboutText));
}

void InviwoMainWindow::exitInviwo() {
    InviwoApplication::getPtr()->closeInviwoApplication();
    QMainWindow::close();
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