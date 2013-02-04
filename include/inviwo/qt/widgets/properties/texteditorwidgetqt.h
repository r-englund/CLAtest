#ifndef IVW_TEXTEDITORWIDGETQT_H
#define IVW_TEXTEDITORWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QFile>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QTextStream>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/texteditorproperty.h>

namespace inviwo{

class IVW_QTWIDGETS_API TextEditorWidgetQt : public PropertyWidgetQt{
    
    Q_OBJECT;

public:
    TextEditorWidgetQt(TextEditorProperty* property);

    void updateFromProperty();

private:
    TextEditorProperty* property_;
    QLineEdit* lineEdit_;
    QToolButton* openButton_;
    QPlainTextEdit* textEditor_;
    QPushButton*   editButton_;
    QFile* file_;

    void generateWidget();

    public slots:
        void setPropertyValue();
        void editFile();
        void writeToFile();
};



}//namespace

#endif //IVW_TEXTEDITORWIDGETQT_H