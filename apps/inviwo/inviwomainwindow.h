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
#include <inviwo/qt/editor/resourcemanagerwidget.h>

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
    
    /** 
     * \brief loads and unloads external workspace while current workspace is still active
     *
     * This function is used to get snapshot of all canvases in a given external workspace.
     * This function safely loads an external network without conflicting with current loaded workspace.
     * And also safely clears them.
     * 
     * @param std::string workspaceFileName external work space
     * @return std::vector<std::string> file locations where snapshots of canvases in external workspace are save.
     */
    std::vector<std::string> getWorkspaceSnapshots(std::string workspaceFileName);

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
    ResourceManagerWidget* resourceManagerWidget_;

    // mainwindow menus
    QMenuBar* basicMenuBar;
    QMenu* fileMenuItem_;
    QMenu* helpMenuItem_;
    QMenu* viewMenuItem_;
    QMenu* viewModeItem_;
    
    // mainwindow menuactions
    QAction* newFileAction_;
    QAction* openFileAction_;
    QAction* saveFileAction_;
    QAction* saveAsFileAction_;
    QAction* recentFileSeparator_;
    QAction* recentFileActions_[maxNumRecentFiles_];
    QAction* developerViewModeAction_;
    QAction* applicationViewModeAction_;

    // settings
    bool lastExitWithoutErrors_;
    bool workspaceModified_;

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
