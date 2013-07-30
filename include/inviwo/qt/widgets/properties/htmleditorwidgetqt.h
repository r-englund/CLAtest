#ifndef IVW_HTMLEDITORYWIDGETQT_H
#define IVW_HTMLEDITORYWIDGETQT_H

//QT includes
#include <QFile>
#include <QCheckBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>
#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>

#include <inviwo/qt/widgets/properties/htmllistwidgetqt.h>

namespace inviwo{
    
class TextEditorWidgetQt;
class IVW_QTWIDGETS_API HtmlEditorWidgetQt : public QWidget{

    Q_OBJECT;

public:
    HtmlEditorWidgetQt();
    bool saveDialog();
    void setParent(TextEditorWidgetQt*);

    QFile* file_;
    QTextEdit* htmlEditor_;
    QTextEdit* htmlOutput_;
    TextEditorWidgetQt* mainParentWidget_;
    QToolBar* toolBar_;
    QToolButton *runButton_;    
    QToolButton *saveButton_;    
    QToolButton *reLoadButton_;    
    QWidget* mainWidget_;
    std::string tmpPropertyValue_;
    HtmlTreeWidget* htmlTreeWidgetQt_;


    void generateWidget();

public slots:
        void run();

protected:
    void closeEvent(QCloseEvent *);
    /*void showEvent(QShowEvent *);*/
};

}//namespace

#endif //IVW_HTMLEDITORYWIDGETQT_H