#ifndef IVW_INVIWOMAINWINDOW_H
#define IVW_INVIWOMAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QToolBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>


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

public slots:
    void newNetwork();
    void openNetwork();
    void openRecentNetwork();
    void saveNetwork();
    void saveNetworkAs();

private:
    void openNetwork(QString networkFileName);
    void addToolBars();
    void addMenus();
    void addMenuActions();
    void closeEvent(QCloseEvent *event);

    void addToRecentNetworks(QString networkFileName);
    void updateRecentNetworks();
    void setCurrentNetwork(QString currentNetworkFileName);
    void updateWindowTitle();

    //TODO: Currently for testing, save the NetworkEditorView which consists entire network processors
    NetworkEditorView* networkEditorView_;

    //MainWindow ToolBars
    QToolBar* basicToolbar_;

    //MainWindow Menus
    QMenuBar* basicMenuBar;
    QMenu* fileMenuItem_;
    QMenu* helpMenuItem_;
    QMenu* viewMenuItem_;
    
    //MainWindow MenuActions
    QAction* newFileAction_;
    QAction* openFileAction_;
    QAction* saveFileAction_;
    QAction* saveAsFileAction_;
    QAction* recentFileSeparator_;
    QAction* recentFileActions_[maxNumRecentFiles_];

    //Paths
    QString rootDir_;
    QString networkFileDir_;
    QString currentNetworkFileName_;
    QStringList recentFileList_;

    CanvasQt* defaultRenderContext_;
};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
