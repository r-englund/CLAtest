/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
#ifdef IVW_PYTHON_QT
#include <modules/pythonqt/pythoneditorwidget.h>
#endif
#include <inviwo/qt/editor/networkeditor.h>

namespace inviwo {

class NetworkEditorView;
class NetworkEditor;
class CanvasQt;

class InviwoMainWindow : public QMainWindow, 
                         public VoidObserver, 
                         public NetworkEditorObserver {
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
    bool processEndCommandLineArgs();
    
    void openWorkspace(QString workspaceFileName);

    std::string getCurrentWorkspace();

    virtual void notify();

    virtual void onNetworkEditorFileChanged(const std::string &filename);

public slots:
    void newWorkspace();
    void openWorkspace();
    void openRecentWorkspace();
    void saveWorkspace();
    void saveWorkspaceAs();
    void exitInviwo();
    void disableEvaluation(bool);

private:    
    void addMenus();
    void addMenuActions();
	void addToolBars();
    void closeEvent(QCloseEvent *event);

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
#ifdef IVW_PYTHON_QT
    PythonEditorWidget* pythonEditorWidget_;
#endif

    // mainwindow menus
    QMenuBar* basicMenuBar;
    QMenu* fileMenuItem_;
    QMenu* helpMenuItem_;
    QMenu* viewMenuItem_;
    QMenu* viewModeItem_;
#ifdef IVW_PYTHON_QT
    QMenu* pythonMenuItem_;
#endif
    
    // mainwindow menuactions
    QAction* newFileAction_;
    QAction* openFileAction_;
    QAction* saveFileAction_;
    QAction* saveAsFileAction_;
    QAction* exitAction_;
    QAction* recentFileSeparator_;
    QAction* recentFileActions_[maxNumRecentFiles_];
    QAction* developerViewModeAction_;
    QAction* applicationViewModeAction_;
#ifdef IVW_PYTHON_QT
    QAction* pythonEditorOpenAction_;
#endif

    QToolButton* enableDisableEvaluationButton_;

	// mainwindow toolbars
	QToolBar* fileToolBar_;
	QToolBar* viewToolBar_;

    // settings
    bool lastExitWithoutErrors_;
    bool workspaceModified_;
    bool maximized_;

    // paths
    QString rootDir_;
    QString workspaceFileDir_;
    QString currentWorkspaceFileName_;
    QStringList recentFileList_;

    // the default render context for managing render states
    CanvasQt* defaultRenderContext_;


};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
