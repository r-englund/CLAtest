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

class InviwoMainWindow : public QMainWindow {
Q_OBJECT
public:
    static const unsigned int maxNumRecentFiles_ = 10;

    InviwoMainWindow();
    ~InviwoMainWindow();

    virtual void initializeAndShow();
    virtual void deinitialize();
    virtual void initializeWorkspace();

    void openLastNetwork();

public slots:
    void newNetwork();
    void openNetwork();
    void openRecentNetwork();
    void saveNetwork();
    void saveNetworkAs();

private:
    void openNetwork(QString networkFileName);
    void addMenus();
    void addMenuActions();
    void closeEvent(QCloseEvent *event);

    void addToRecentNetworks(QString networkFileName);
    void updateRecentNetworks();
    void setCurrentNetwork(QString currentNetworkFileName);
    void updateWindowTitle();

    NetworkEditorView* networkEditorView_;

    // mainwindow toolbar
    QToolBar* basicToolbar_;

    // dock widgets
    SettingsWidget* settingsWidget_;
    ProcessorListWidget* processorListWidget_;
    PropertyListWidget* propertyListWidget_;
    ConsoleWidget* consoleWidget_;
    MappingWidget* mappingwidget_;

    // mainwindow menus
    QMenuBar* basicMenuBar;
    QMenu* fileMenuItem_;
    QMenu* helpMenuItem_;
    QMenu* viewMenuItem_;
    
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

    // paths
    QString rootDir_;
    QString networkFileDir_;
    QString currentNetworkFileName_;
    QStringList recentFileList_;

    // the default render context for managing render sates
    CanvasQt* defaultRenderContext_;
};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
