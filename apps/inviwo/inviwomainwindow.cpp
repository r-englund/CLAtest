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
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/qt/editor/networkeditorview.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/util/commandlineparser.h>

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

void InviwoMainWindow::showWindow() {
    if (maximized_) showMaximized();
    else show();
};

void InviwoMainWindow::deinitialize() {
}

void InviwoMainWindow::initializeWorkspace() {
    ProcessorNetwork* processorNetwork = const_cast<ProcessorNetwork*>(networkEditorView_->getNetworkEditor()->getProcessorNetwork());
    VoidObserver::addObservation(processorNetwork);
    processorNetwork->addObserver(this);
}

void InviwoMainWindow::notify() {
    workspaceModified_ = true;
    updateWindowTitle();
}

bool InviwoMainWindow::processCommandLineArgs() {
    const CommandLineParser* cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();
#ifdef IVW_PYTHON_QT

    if (cmdparser->getRunPythonScriptAfterStartup()) {
        PythonEditorWidget::getPtr()->show();
        PythonEditorWidget::getPtr()->loadFile(cmdparser->getPythonScriptName(),false);
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
    menuBar_ = menuBar();
    QAction* first = 0;

    if (menuBar_->actions().size() > 0)
        first = menuBar_->actions()[0];

    fileMenuItem_ = new QMenu(tr("&File"),menuBar_);
    viewMenuItem_ = new QMenu(tr("&View"),menuBar_);
    viewModeItem_ = new QMenu(tr("&View mode"),menuBar_);
    menuBar_->insertMenu(first, fileMenuItem_);
    menuBar_->insertMenu(first, viewMenuItem_);
    viewMenuItem_->addMenu(viewModeItem_);
    helpMenuItem_ = menuBar_->addMenu(tr("&Help"));
}

void InviwoMainWindow::addMenuActions() {
    // file menu entries
    workspaceActionNew_ = new QAction(QIcon(":/icons/new.png"), tr("&New Workspace"), this);
    workspaceActionNew_->setShortcut(QKeySequence::New);
    connect(workspaceActionNew_, SIGNAL(triggered()), this, SLOT(newWorkspace()));
    fileMenuItem_->addAction(workspaceActionNew_);
    workspaceActionOpen_ = new QAction(QIcon(":/icons/open.png"), tr("&Open Workspace"), this);
    workspaceActionOpen_->setShortcut(QKeySequence::Open);
    connect(workspaceActionOpen_, SIGNAL(triggered()), this, SLOT(openWorkspace()));
    fileMenuItem_->addAction(workspaceActionOpen_);
    workspaceActionSave_ = new QAction(QIcon(":/icons/save.png"), tr("&Save Workspace"), this);
    workspaceActionSave_->setShortcut(QKeySequence::Save);
    connect(workspaceActionSave_, SIGNAL(triggered()), this, SLOT(saveWorkspace()));
    fileMenuItem_->addAction(workspaceActionSave_);
    workspaceActionSaveAs_ = new QAction(QIcon(":/icons/saveas.png"), tr("&Save Workspace As"), this);
    workspaceActionSaveAs_->setShortcut(QKeySequence::SaveAs);
    connect(workspaceActionSaveAs_, SIGNAL(triggered()), this, SLOT(saveWorkspaceAs()));
    fileMenuItem_->addAction(workspaceActionSaveAs_);
    recentFileSeparator_ = fileMenuItem_->addSeparator();

    for (int i=0; i<maxNumRecentFiles_; i++) {
        workspaceActionRecent_[i] = new QAction(this);
        workspaceActionRecent_[i]->setVisible(false);
        connect(workspaceActionRecent_[i], SIGNAL(triggered()), this, SLOT(openRecentWorkspace()));
        fileMenuItem_->addAction(workspaceActionRecent_[i]);
    }

    recentFileSeparator_ = fileMenuItem_->addSeparator();
    exitAction_ = new QAction(QIcon(":/icons/button_cancel.png"), tr("&Exit"), this);
    exitAction_->setShortcut(QKeySequence::Close);
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(exitInviwo()));
    fileMenuItem_->addAction(exitAction_);
    // dock widget visibility menu entries
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
    viewModeActionDeveloper_ = new QAction(tr("&Developer View"),this);
    viewModeActionDeveloper_->setCheckable(true);
    viewModeActionDeveloper_->setIcon(QIcon(":/icons/view-developer.png"));
    viewModeItem_->addAction(viewModeActionDeveloper_);
    viewModeActionApplication_ = new QAction(tr("&Application View"),this);
    viewModeActionApplication_->setCheckable(true);
    viewModeActionApplication_->setIcon(QIcon(":/icons/view-application.png"));
    viewModeItem_->addAction(viewModeActionApplication_);
    QActionGroup* viewModeActionGroup = new QActionGroup(this);
    viewModeActionGroup->addAction(viewModeActionDeveloper_);
    viewModeActionGroup->addAction(viewModeActionApplication_);
    PropertyVisibilityMode viewMode = propertyListWidget_->getViewMode();

    if (viewMode == DEVELOPMENT) {
        viewModeActionDeveloper_->setChecked(true);
    } else if (viewMode == APPLICATION) {
        viewModeActionApplication_->setChecked(true);
    }

    connect(viewModeActionDeveloper_, SIGNAL(triggered(bool)), propertyListWidget_, SLOT(setDeveloperViewMode(bool)));
    connect(viewModeActionApplication_, SIGNAL(triggered(bool)), propertyListWidget_, SLOT(setApplicationViewMode(bool)));
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
    workspaceToolBar_ = addToolBar("File");
    workspaceToolBar_->setObjectName("fileToolBar");
    workspaceToolBar_->addAction(workspaceActionNew_);
    workspaceToolBar_->addAction(workspaceActionOpen_);
    workspaceToolBar_->addAction(workspaceActionSave_);
    workspaceToolBar_->addAction(workspaceActionSaveAs_);
    viewModeToolBar_ = addToolBar("View");
    viewModeToolBar_->setObjectName("viewModeToolBar");
    viewModeToolBar_->addAction(viewModeActionDeveloper_);
    viewModeToolBar_->addAction(viewModeActionApplication_);
    viewModeToolBar_->addWidget(enableDisableEvaluationButton_);
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
            workspaceActionRecent_[i]->setText(menuEntry);
            workspaceActionRecent_[i]->setData(recentFileList_[i]);
            workspaceActionRecent_[i]->setVisible(true);
        } else workspaceActionRecent_[i]->setVisible(false);
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

    if (PythonEditorWidget::getPtr()->isActiveWindow() && PythonEditorWidget::getPtr()->hasFocus()) {
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

    if (!file.exists()) {
        LogError("Could not find workspace file: " << workspaceFileName.toLocal8Bit().constData());
        return;
    }

    networkEditorView_->getNetworkEditor()->loadNetwork(workspaceFileName.toLocal8Bit().constData());
    workspaceModified_ = false;
    onNetworkEditorFileChanged(workspaceFileName.toLocal8Bit().constData());
}

void InviwoMainWindow::onNetworkEditorFileChanged(const std::string& filename) {
    setCurrentWorkspace(filename.c_str());
    addToRecentWorkspaces(filename.c_str());
}

void InviwoMainWindow::openLastWorkspace() {
    // if a workspace is defined by an argument, that workspace is opened, otherwise, the last opened workspace is used
    const CommandLineParser* cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();

    if (cmdparser->getLoadWorkspaceFromArg())
        openWorkspace(static_cast<const QString>(cmdparser->getWorkspacePath().c_str()));
    else if (!recentFileList_.isEmpty() && lastExitWithoutErrors_)
        openWorkspace(recentFileList_[0]);
    else
        newWorkspace();
}

void InviwoMainWindow::openWorkspace() {
    if (askToSaveWorkspaceChanges()) {
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

    if (PythonEditorWidget::getPtr()->isActiveWindow() && PythonEditorWidget::getPtr()->hasFocus()) {
        PythonEditorWidget::getPtr()->save();
        return;
    } // only save workspace if python editor does not have focus

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

void InviwoMainWindow::disableEvaluation(bool disable) {
    if (disable)
        networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator()->disableEvaluation();
    else
        networkEditorView_->getNetworkEditor()->getProcessorNetworkEvaluator()->enableEvaluation();
}

void InviwoMainWindow::showAboutBox() {
    std::string aboutText;
    aboutText.append("<b>Inviwo V"+IVW_VERSION+"</b><br>");
    aboutText.append("Interactive Visualization Workshop<br>");
    aboutText.append("&copy; 2012-2014 The Inviwo Foundation<br>");
    aboutText.append("<a href='http://www.inviwo.org/'>http://www.inviwo.org/</a>");
    aboutText.append("<p>Inviwo is a rapid prototyping environment for interactive \
                     visualizations.<br>It is licensed under the Simplified BSD license.</p>");
    aboutText.append("<p><b>Core Team:</b><br>");
    aboutText.append("Rickard Englund, Daniel J&ouml;nsson, Sathish Kottravel, Timo Ropinski, Peter Steneteg, Erik Sund&eacute;n</p>");
    aboutText.append("<p><b>Additional Developers:</b><br>");
    aboutText.append("Alexander Johansson, Johan Noren, Viktor Axelsson</p>");
    aboutText.append("<p><b>Former Developers:</b><br>");
    aboutText.append("Hans-Christian Helltegen, Andreas Valter, Emanuel Winblad</p>");
    QMessageBox::about(this, QString::fromStdString("Inviwo V"+IVW_VERSION), QString::fromStdString(aboutText));
}

void InviwoMainWindow::exitInviwo() {
    QMainWindow::close();
    InviwoApplication::getPtr()->closeInviwoApplication();
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