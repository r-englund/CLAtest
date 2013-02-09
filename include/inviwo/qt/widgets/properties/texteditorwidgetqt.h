#ifndef IVW_TEXTEDITORWIDGETQT_H
#define IVW_TEXTEDITORWIDGETQT_H


#include <QFile>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QTextStream>
#include <QMenuBar>
#include <QMenu>
#include <QWidget>
#include <QToolBar>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/properties/buttonproperty.h>


namespace inviwo{

class IVW_QTWIDGETS_API TextEditorWidgetQt : public PropertyWidgetQt{
    
    Q_OBJECT;

public:
 /**
 * A constructor for the text editor widget
 * @param property a stringproperty or a fileproperty
 * @param showProperty a bool for showing the string- or filepropertywidget
 */
    TextEditorWidgetQt(Property* property,bool showProperty);

    void updateFromProperty();

private:
    bool showProperty_;
    ButtonPropertyWidgetQt* btnWidget_;
    ButtonProperty btnProperty_;
    FilePropertyWidgetQt* fileWidget_;
    Property* property_;
    QPlainTextEdit* textEditor_;
    QFile* file_;
    StringPropertyWidgetQt* stringWidget_;
    std::string tmpPropertyValue_;
    //QMenuBar* menuBar_;
    //QMenu*  fileMenu_;
    QWidget* mainWidget_;
    QToolBar* toolBar_;
    QToolButton *saveButton_;
    

    void generateWidget();

    public slots:
        void setPropertyValue();
        void editFile();
        void editString();
        void writeToFile();
        void writeToString();
};



}//namespace

#endif //IVW_TEXTEDITORWIDGETQT_H