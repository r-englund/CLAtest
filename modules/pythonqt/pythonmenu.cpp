#include "pythonmenu.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include "pythoninfowidget.h"
#include "pythoneditorwidget.h"

namespace inviwo {

    PythonMenu::PythonMenu(){
        QMainWindow* win = static_cast<InviwoApplicationQt*>(InviwoApplication::getPtr())->getMainWindow();

       
        auto menu = win->menuBar()->addMenu("Python");
        QAction* pythonEditorOpen = menu->addAction(QIcon(":/icons/python.png"),"&Python Editor");
        QAction* infoAction = menu->addAction("Info");

        infoWidget_ = new PythonInfoWidget(win);
        pythonEditorWidget_ = new PythonEditorWidget(win);


        win->connect(pythonEditorOpen,SIGNAL(triggered(bool)),PythonEditorWidget::getPtr(),SLOT(show(void)));
        win->connect(infoAction,SIGNAL(triggered(bool)),infoWidget_,SLOT(show(void)));

    }

    PythonMenu::~PythonMenu(){
       // infoWidget_->deleteLater();
    }

} // namespace

