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

    class InviwoMainWindow : public QMainWindow {
Q_OBJECT
    public:
        InviwoMainWindow();
        ~InviwoMainWindow();

    public slots:
        bool saveNetwork(); 
        bool loadNetwork(); 

    private:
        void addToolBars();
        void addMenus();
        void addMenuActions();
        void closeEvent(QCloseEvent *event);


        //TODO: Currently for testing, save the NetworkEditorView which consists entire network processors
        NetworkEditorView* networkEditorView_;

        //MainWindow ToolBars
        QToolBar *basicToolbar_;

        //MainWindow Menus
        QMenuBar *basicMenuBar;
        QMenu *fileMenuItem_;
        QMenu *editMenuItem_;
        
        //MainWindow MenuActions
        QAction *openFileAction_;
        QAction *saveFileAction_;

        //Paths
        QString rootDir_;
        QString networkFileDir_;

};

} // namespace

#endif // IVW_INVIWOMAINWINDOW_H
