#ifndef IVW_INVIWOMAINWINDOW_H
#define IVW_INVIWOMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include <inviwo/qt/editor/mappingwidget.h>
#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/qt/editor/consolewidget.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>

namespace inviwo {

class NetworkEditorView;
class NetworkEditor;
class CanvasQt;

class InviwoMainWindow : public QMainWindow, public VoidObserver {
Q_OBJECT;
public:
    static const unsigned int maxNumRecentFiles_ = 10;

    InviwoMainWindow();
    ~InviwoMainWindow();

    virtual void initializeAndShow();
    virtual void deinitialize();
    virtual void initializeWorkspace();

    void openLastWorkspace();
    bool processEndCommandLineArgs();

    bool isSoundsOn();

    virtual void notify();

public slots:
    void newWorkspace();
    void openWorkspace();
    void openRecentWorkspace();
    void saveWorkspace();
    void saveWorkspaceAs();

private:
    void openWorkspace(QString workspaceFileName);
    void addMenus();
    void addMenuActions();
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

    // mainwindow menus
    QMenuBar* basicMenuBar;
    QMenu* fileMenuItem_;
    QMenu* helpMenuItem_;
    QMenu* viewMenuItem_;
    QMenu* optionMenuItem_;
    
    // mainwindow menuactions
    QAction* newFileAction_;
    QAction* openFileAction_;
    QAction* saveFileAction_;
    QAction* saveAsFileAction_;
    QAction* recentFileSeparator_;
    QAction* recentFileActions_[maxNumRecentFiles_];
    QAction* settingsWidgetViewAction_;
    QAction* processorListWidgetViewAction_;
    QAction* propertyListWidgetViewAction_;
    QAction* consoleWidgetViewAction_;
    QAction* mappingwidgetviewAction_;
    QAction* soundOnOffAction_;

    // settings
    bool lastExitWithoutErrors_;
    bool soundsOn_;
    bool workspaceModified_;

    // paths
    QString rootDir_;
    QString workspaceFileDir_;
    QString currentWorkspaceFileName_;
    QStringList recentFileList_;

    // the default render context for managing render sates
    CanvasQt* defaultRenderContext_;
};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
