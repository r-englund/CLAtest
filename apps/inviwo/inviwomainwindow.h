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
 * Main file authors: Timo Ropinski, Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_INVIWOMAINWINDOW_H
#define IVW_INVIWOMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include <inviwo/qt/editor/mappingwidget.h>
#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/qt/editor/consolewidget.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/qt/editor/resourcemanagerwidget.h>
#include <inviwo/qt/editor/networkeditor.h>

namespace inviwo {

class NetworkEditorView;
class NetworkEditor;

class InviwoMainWindow : public QMainWindow,
    public PropertyListWidgetObserver,
    public NetworkEditorObserver,
    public ProcessorNetworkObserver {
    Q_OBJECT
public:
    static const unsigned int maxNumRecentFiles_ = 10;

    InviwoMainWindow();
    ~InviwoMainWindow();

    virtual void initialize();
    virtual void showWindow();
    virtual void deinitialize();
    virtual void initializeWorkspace();

    void openLastWorkspace();
    void openWorkspace(QString workspaceFileName);
    std::string getCurrentWorkspace();

    bool processCommandLineArgs();

    virtual void onProcessorNetworkChange();
    virtual void onNetworkEditorFileChanged(const std::string& filename);

public slots:
    void newWorkspace();
    void openWorkspace();
    void openRecentWorkspace();
    void saveWorkspace();
    void saveWorkspaceAs();
    void exitInviwo();
    void disableEvaluation(bool);
    void showAboutBox();

private:
    void addMenus();
    void addMenuActions();
    void addToolBars();
    void closeEvent(QCloseEvent* event);

    bool askToSaveWorkspaceChanges();

    void addToRecentWorkspaces(QString workspaceFileName);
    void updateRecentWorkspaces();
    void setCurrentWorkspace(QString workspaceFileName);

    void updateWindowTitle();

    NetworkEditorView* networkEditorView_;

    // mainwindow toolbar
    QToolBar* basicToolbar_;

    // dock widgets
    SettingsWidget* settingsWidget_;
    ProcessorTreeWidget* processorTreeWidget_;
    PropertyListWidget* propertyListWidget_;
    ConsoleWidget* consoleWidget_;
    MappingWidget* mappingwidget_;
    ResourceManagerWidget* resourceManagerWidget_;

    // mainwindow menus
    QMenuBar* menuBar_;
    QMenu* fileMenuItem_;
    QMenu* viewMenuItem_;
    QMenu* viewModeItem_;
    QMenu* helpMenuItem_;

    // mainwindow menuactions
    QAction* workspaceActionNew_;
    QAction* workspaceActionOpen_;
    QAction* workspaceActionSave_;
    QAction* workspaceActionSaveAs_;
    QAction* exitAction_;
    QAction* recentFileSeparator_;
    QAction* workspaceActionRecent_[maxNumRecentFiles_];
    QAction* viewModeActionDeveloper_;
    QAction* viewModeActionApplication_;
    QAction* aboutBoxAction_;
    QToolButton* enableDisableEvaluationButton_;

    // mainwindow toolbars
    QToolBar* workspaceToolBar_;
    QToolBar* viewModeToolBar_;

    // settings
    bool lastExitWithoutErrors_;
    bool workspaceModified_;
    bool maximized_;

    // paths
    QString rootDir_;
    QString workspaceFileDir_;
    QString currentWorkspaceFileName_;
    QStringList recentFileList_;
};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
