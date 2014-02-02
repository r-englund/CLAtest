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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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

namespace inviwo {

class TextEditorWidgetQt;
class IVW_QTWIDGETS_API HtmlEditorWidgetQt : public QWidget {

    Q_OBJECT

public:
    HtmlEditorWidgetQt();
    bool saveDialog();
    void setParent(TextEditorWidgetQt*);

    QFile* file_;
    QTextEdit* htmlEditor_;
    QTextEdit* htmlOutput_;
    TextEditorWidgetQt* mainParentWidget_;
    QToolBar* toolBar_;
    QToolButton* runButton_;
    QToolButton* saveButton_;
    QToolButton* reLoadButton_;
    std::string tmpPropertyValue_;
    HtmlTreeWidget* htmlTreeWidgetQt_;


    void generateWidget();

public slots:
    void run();

protected:
    void closeEvent(QCloseEvent*);
    /*void showEvent(QShowEvent *);*/
};

}//namespace

#endif //IVW_HTMLEDITORYWIDGETQT_H