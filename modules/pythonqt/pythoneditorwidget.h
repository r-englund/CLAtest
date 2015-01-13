/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2013-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#ifndef IVW_PYTHONEDITORYWIDGET_H
#define IVW_PYTHONEDITORYWIDGET_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <modules/python/pythonscript.h>
#include <modules/python/pythonmodule.h>
#include <modules/python/pythonexecutionoutputobeserver.h>
#include <inviwo/core/network/processornetworkobserver.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <QTextEdit>
#include <QColor>
#include <QToolButton>
#include <QSettings>

class QPlainTextEdit;

namespace inviwo {
class SyntaxHighligther;
class IVW_MODULE_PYTHONQT_API PythonEditorWidget : public InviwoDockWidget,
                                                   public FileObserver,
                                                   public PythonExecutionOutputObeserver,
                                                   public ProcessorNetworkObserver {
    Q_OBJECT

public:
    PythonEditorWidget(QWidget* parent);
    virtual ~PythonEditorWidget();

    void appendToOutput(const std::string& msg, bool error = false);
    virtual void fileChanged(std::string fileName);
    void loadFile(std::string fileName, bool askForSave = true);

    virtual void onPyhonExecutionOutput(const std::string &msg,const PythonExecutionOutputStream &outputType);

    virtual void onProcessorNetworkChange();
    bool hasFocus() const;
    static PythonEditorWidget* getPtr();

private:
    void buildWidget();

    QSettings settings_;
    QPlainTextEdit* pythonCode_;
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

    static PythonEditorWidget* instance_;

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

}  // namespace

#endif  // IVW_PYTHONEDITORYWIDGET_H