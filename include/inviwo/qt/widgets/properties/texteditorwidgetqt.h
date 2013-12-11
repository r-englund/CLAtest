/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_TEXTEDITORWIDGETQT_H
#define IVW_TEXTEDITORWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

//QT includes
#include <QWidget>

//Property includes
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/property.h>
#include "propertywidgetqt.h"


class QFile;
class QTextEdit;
class QToolBar;
class QToolButton;
class QCheckBox;

namespace inviwo{
	class SyntaxHighligther;
	class FilePropertyWidgetQt;
	class TextEditorWidgetQt;
	class HtmlEditorWidgetQt;
	class StringPropertyWidgetQt;

class IVW_QTWIDGETS_API ModifiedWidget : public QWidget{

    Q_OBJECT

public:
    ModifiedWidget();
    bool saveDialog();
    void setParent(TextEditorWidgetQt*);

	QFile* file_;
	QTextEdit* textEditor_;
	TextEditorWidgetQt* mainParentWidget_;
	QToolBar* toolBar_;
	QToolButton* reDoButton_;
	QToolButton* reLoadButton_;
	QToolButton* saveButton_;
	QToolButton* unDoButton_;
	QWidget* mainWidget_;
	std::string tmpPropertyValue_;

	SyntaxHighligther* getSyntaxHighligther();

    void generateWidget();

public slots:
	void textHasChanged();

protected:
    void closeEvent(QCloseEvent *);
    /*void showEvent(QShowEvent *);*/

private:
	SyntaxHighligther* syntaxHighligther_;
};

class IVW_QTWIDGETS_API TextEditorWidgetQt : public PropertyWidgetQt{
    
    Q_OBJECT

public:
 
    TextEditorWidgetQt(Property* property);

    void updateFromProperty();
	SyntaxHighligther* getSyntaxHighligther();
public:
     bool saveDialog();


private:
    QToolButton* btnEdit_;
    FilePropertyWidgetQt* fileWidget_;
    Property* property_;
    QFile* file_;
    ModifiedWidget* textEditorWidget_;
    HtmlEditorWidgetQt* htmlEditorWidget_;
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