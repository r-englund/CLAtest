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
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#ifndef IVW_PYTHONEDITORYWIDGET_H
#define IVW_PYTHONEDITORYWIDGET_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <modules/python/pythonscript.h>
#include <modules/python/pythonmodule.h>
#include <modules/python/pythonexecutionoutputobeserver.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <QTextEdit>
#include <QColor>
#include <QToolButton>

class QPlainTextEdit;

namespace inviwo{
    class SyntaxHighligther;
class IVW_MODULE_PYTHONQT_API PythonEditorWidget : public InviwoDockWidget 
                                                 , public FileObserver 
                                                 , public PythonExecutionOutputObeserver
                                                 , public VoidObserver {
    Q_OBJECT

public:
    PythonEditorWidget(QWidget* parent);
    virtual ~PythonEditorWidget();

    void appendToOutput(const std::string &msg,bool error = false);
    virtual void fileChanged(std::string fileName);
    void loadFile(std::string fileName,bool askForSave = true);

	virtual void onPyhonExecutionOutput(std::string msg,OutputType outputType);

    virtual void notify();

private:
    void buildWidget();

    QTextEdit* pythonCode_;
    QTextEdit* pythonOutput_;

	QColor infoTextColor_;
	QColor errorTextColor_;

    PythonScript script_;
    std::string scriptFileName_;

    bool unsavedChanges_;
    void readFile();

    QToolButton* startRecordScriptButton_;
    QToolButton* endRecordScriptButton_;

    SyntaxHighligther* syntaxHighligther_;

public slots:
    void save();
    void saveAs();
    void open();
    void run();
    void show();
    void setDefaultText();
    void clearOutput();
    void startRecordingScript();
    void endRecordingScript();
    void onTextChange();
};

}//namespace

#endif //IVW_PYTHONEDITORYWIDGET_H