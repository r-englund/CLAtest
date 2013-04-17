#ifndef IVW_TEXTEDITORWIDGETQT_H
#define IVW_TEXTEDITORWIDGETQT_H

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


//Property includes
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/property.h>

//Widget includes
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>

//Inviwo
#include <inviwo/core/common/inviwoapplication.h>


namespace inviwo{
class TextEditorWidgetQt;
class IVW_QTWIDGETS_API ModifiedWidget : public QWidget{

    Q_OBJECT;

public:
    ModifiedWidget();
    bool saveDialog();
    void setParent(TextEditorWidgetQt*);

    QFile* file_;
    QPlainTextEdit* textEditor_;
    TextEditorWidgetQt* mainParentWidget_;
    QToolBar* toolBar_;
    QToolButton *reDoButton_;
    QToolButton *reLoadButton_;
    QToolButton *saveButton_;
    QToolButton *unDoButton_;
    QWidget* mainWidget_;
    std::string tmpPropertyValue_;


    void generateWidget();
protected:
    void closeEvent(QCloseEvent *);
    /*void showEvent(QShowEvent *);*/
};

class IVW_QTWIDGETS_API TextEditorWidgetQt : public PropertyWidgetQt{
    
    Q_OBJECT;

public:
 
    TextEditorWidgetQt(Property* property);

    void updateFromProperty();

public:
     bool saveDialog();


private:
    QToolButton* btnEdit_;
    FilePropertyWidgetQt* fileWidget_;
    Property* property_;
    QFile* file_;
    ModifiedWidget* textEditorWidget_;
    StringPropertyWidgetQt* stringWidget_;
    std::string tmpPropertyValue_;
    QCheckBox* checkBox_;

    void generateWidget();
   
public slots:
    void loadFile();
    void loadString();
    void editFile();
    void editString();
    void setPropertyValue();
    bool writeToFile();
    bool writeToString();

};


}//namespace

#endif //IVW_TEXTEDITORWIDGETQT_H