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

#include <modules/pythonqt/pythoneditorwidget.h>
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/filedirectory.h>
#include <modules/python/pythonmodule.h>
#include <QCommandLinkButton>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QButtonGroup>
#include <QToolButton>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QFrame>
#include <inviwo/core/util/clock.h>

namespace inviwo{
    PythonEditorWidget::PythonEditorWidget(QWidget* parent): 
            InviwoDockWidget(tr("Python Editor"), parent),
            VoidObserver(),
            script_(),
            unsavedChanges_(false),
        infoTextColor_(153,153,153), errorTextColor_(255,107,107) {
        setObjectName("PythonEditor");
        setAllowedAreas(Qt::AllDockWidgetAreas);
        setFloating(true);

        buildWidget();
        resize(500,700);

        setVisible(false);  

        ProcessorNetwork* processorNetwork = InviwoApplication::getPtr()->getProcessorNetwork();
        addObservation(processorNetwork);
        processorNetwork->addObserver(this);
    }   

    void PythonEditorWidget::notify() {
        if (script_.getScriptRecorder()->isRecording()) {
            script_.getScriptRecorder()->recordNetworkChanges();

            std::string currentScriptInEditor = pythonCode_->toPlainText().toLocal8Bit().constData();

            if (script_.getSource()!=currentScriptInEditor) {
                //set new script source from recorder
                clearOutput();
                pythonCode_->setText(tr(script_.getSource().c_str()));
                pythonCode_->setText(tr(script_.getSource().c_str()));
                unsavedChanges_ = true;
            }
        }
    }

    void PythonEditorWidget::buildWidget(){
        setWindowIcon(QIcon(":/icons/python.png"));
    
        QWidget* content = new QWidget(this);
        QVBoxLayout *verticalLayout = new QVBoxLayout(content);

        ////Create Buttons
        QHBoxLayout *horizontalLayout = new QHBoxLayout();

        QToolButton* runButton = new QToolButton(content);
        QToolButton* newButton = new QToolButton(content);
        QToolButton* openButton = new QToolButton(content);
        QToolButton* saveButton = new QToolButton(content);
        QToolButton* saveAsButton = new QToolButton(content);
        QPushButton* clearOutputButton = new QPushButton(content);
        startRecordScriptButton_ = new QToolButton(content);
        endRecordScriptButton_ = new QToolButton(content);

        runButton->setIcon(QIcon(":/icons/python.png"));
        runButton->setToolTip("Compile and run");

        newButton->setIcon(QIcon(":/icons/new.png"));
        newButton->setToolTip("New file");

        openButton->setIcon(QIcon(":/icons/open.png"));
        openButton->setToolTip("Open Python script");

        saveButton->setIcon(QIcon(":/icons/save.png"));
        saveButton->setToolTip("Save");

        saveAsButton->setIcon(QIcon(":/icons/saveas.png"));
        saveAsButton->setToolTip("Save as");
        
        clearOutputButton->setText("Clear Output");   

        startRecordScriptButton_->setIcon(QIcon(":/icons/player_record.png"));
        startRecordScriptButton_->setToolTip("Start Recording");

        endRecordScriptButton_->setIcon(QIcon(":/icons/player_stop.png"));
        endRecordScriptButton_->setToolTip("Stop Recording");
        endRecordScriptButton_->setDisabled(true);
        
        QFrame* line1 = new QFrame(content);
        line1->setFrameShape(QFrame::VLine);
        line1->setFrameShadow(QFrame::Sunken);

        QFrame* line2 = new QFrame(content);
        line2->setFrameShape(QFrame::VLine);
        line2->setFrameShadow(QFrame::Sunken);

        QFrame* line3 = new QFrame(content);
        line3->setFrameShape(QFrame::VLine);
        line3->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(runButton);
        horizontalLayout->addWidget(line1);
        horizontalLayout->addWidget(newButton);
        horizontalLayout->addWidget(openButton);
        horizontalLayout->addWidget(saveButton);
        horizontalLayout->addWidget(saveAsButton);
        horizontalLayout->addWidget(line2);
        horizontalLayout->addWidget(clearOutputButton);
        horizontalLayout->addWidget(line3);
        horizontalLayout->addWidget(startRecordScriptButton_);
        horizontalLayout->addWidget(endRecordScriptButton_);
        
        QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        horizontalLayout->addItem(horizontalSpacer);
  
        //Done creating buttons        
        QSplitter* splitter = new  QSplitter(content);
        splitter->setOrientation(Qt::Vertical);

        pythonCode_ = new QTextEdit(content);
        pythonCode_->setObjectName("pythonEditor");
        
        setDefaultText();       

        pythonOutput_ = new QTextEdit(content);
        pythonOutput_->setObjectName("pythonConsole");
        pythonOutput_->setReadOnly(true);
        pythonOutput_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        
        verticalLayout->addLayout(horizontalLayout);
        
        splitter->addWidget(pythonCode_);
        splitter->addWidget(pythonOutput_);
        splitter->setStretchFactor(0, 1);
        splitter->setStretchFactor(1, 0);

        verticalLayout->addWidget(splitter);

        setWidget(content);
        
        connect(pythonCode_,SIGNAL(textChanged()),this,SLOT(onTextChange()));
        connect(runButton,SIGNAL(clicked()),this,SLOT(run()));
        connect(newButton,SIGNAL(clicked()),this,SLOT(setDefaultText()));
        connect(openButton,SIGNAL(clicked()),this,SLOT(open()));
        connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
        connect(saveAsButton,SIGNAL(clicked()),this,SLOT(saveAs()));
        connect(clearOutputButton,SIGNAL(clicked()),this,SLOT(clearOutput()));
        connect(startRecordScriptButton_,SIGNAL(clicked()),this,SLOT(startRecordingScript()));
        connect(endRecordScriptButton_,SIGNAL(clicked()),this,SLOT(endRecordingScript()));
        
    }


    PythonEditorWidget::~PythonEditorWidget(){}

    void PythonEditorWidget::appendToOutput(const std::string &msg,bool error){
        pythonOutput_->setTextColor(error ? errorTextColor_ : infoTextColor_);
        pythonOutput_->append(msg.c_str());
        
    }

    void PythonEditorWidget::fileChanged(std::string fileName){
        std::string msg = "The file " + URLParser::getFileNameWithExtension(scriptFileName_) + " has been modified outside of Inwivo, do you want to reload its contents";
        int ret = QMessageBox::question(this,"Python Editor",msg.c_str(),"Yes","No");
        if(ret == 0){//yes
            readFile();
        }else{
            unsavedChanges_ = true; //set code change to true so that we can quick save without making more changes
        }
    }

    void PythonEditorWidget::loadFile(std::string fileName,bool askForSave){
        if(askForSave && unsavedChanges_){
            int ret = QMessageBox::information(this,"Python Editor","Do you want to save unsaved changes?","Save","Discard","Cancel");
            if(ret == 0)
                save();
            if(ret == 2) //Cancel
                return;
        }
        scriptFileName_ = fileName;
        readFile();
    }

	void PythonEditorWidget::onPyhonExecutionOutput(std::string msg,OutputType outputType){
		appendToOutput(msg,outputType!=PythonExecutionOutputObeserver::standard);
	}

    void PythonEditorWidget::save(){
        if(scriptFileName_.size()==0){
            saveAs();
        }else if(unsavedChanges_){
            stopFileObservation(scriptFileName_);
            std::ofstream file(scriptFileName_.c_str());
            file << pythonCode_->toPlainText().toLocal8Bit().constData();
            file.close();
            startFileObservation(scriptFileName_);
            LogInfo("Python Script saved("<<scriptFileName_<<")");

            unsavedChanges_ = false;
        }
    }

    void PythonEditorWidget::readFile(){
        std::ifstream file(scriptFileName_.c_str());
        
        std::string text((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
        file.close();

        pythonCode_->setText(text.c_str());
		script_.setSource(text);
        unsavedChanges_ = false;
    }

    void PythonEditorWidget::saveAs(){
        QStringList extensions;
        extensions << "Python Script (*.py)";

        QFileDialog saveFileDialog(this, tr("Save Python Script ..."));
        //TODO consider added scripts folder and use getPath instead of getBasePath
        //or to use workspace directory
        saveFileDialog.setDirectory(QDir((InviwoApplication::getPtr()->getBasePath() + "/data/scripts").c_str()));
        saveFileDialog.setFileMode(QFileDialog::AnyFile);
        saveFileDialog.setAcceptMode(QFileDialog::AcceptSave);
        saveFileDialog.setConfirmOverwrite(true);
        saveFileDialog.setNameFilters(extensions);

        if(saveFileDialog.exec()){
            stopFileObservation(scriptFileName_);
            QString path = saveFileDialog.selectedFiles().at(0);
            if (!path.endsWith(".py")) path.append(".py");
            scriptFileName_ = path.toLocal8Bit().constData();
            save();
        }
    }

    void PythonEditorWidget::open(){
        if(unsavedChanges_){
            int ret = QMessageBox::information(this,"Python Editor","Do you want to save unsaved changes?","Save","Discard","Cancel");
            if(ret == 0)
                save();
            if(ret == 2) //Cancel
                return;
        }

        QStringList extensions;
        extensions << "Python Script (*.py)";

        QFileDialog openFileDialog(this, tr("Open Python Script ..."));
        //TODO consider added scripts folder and use getPath instead of getBasePath
        openFileDialog.setDirectory(QDir((InviwoApplication::getPtr()->getBasePath() + "/data/scripts").c_str()));
        openFileDialog.setFileMode(QFileDialog::AnyFile);
        openFileDialog.setNameFilters(extensions);

        if (openFileDialog.exec()) {
            stopFileObservation(scriptFileName_);
            scriptFileName_ = openFileDialog.selectedFiles().at(0).toLocal8Bit().constData();
            startFileObservation(scriptFileName_);
            readFile();
        }
    }

    void PythonEditorWidget::run(){
        if(unsavedChanges_ && scriptFileName_.size()!=0) //save if needed
            save();
        clearOutput();

        Clock c;
        c.start();
        bool ok = script_.run();
        c.stop();
        if(ok){
            LogInfo("Python Script Executed succesfully");
        }
        LogInfo("Execution time: " << c.getElapsedMiliseconds() << " ms");
    }
    
    void PythonEditorWidget::show(){
        //setFloating(true);
        setVisible(true);
    }

    void PythonEditorWidget::setDefaultText(){
        if(unsavedChanges_){
            int ret = QMessageBox::information(this,"Python Editor","Do you want to save unsaved changes?","Save","Discard Changes","Cancel");
            if(ret == 0)
                save();
            else if(ret == 2) //cancel
                return;
        }
        const static std::string defaultSource = "# Inviwo Python script \nimport inviwo \n\ninviwo.info() \n";
        pythonCode_->setText(defaultSource.c_str());
        script_.setSource(defaultSource);
        stopFileObservation(scriptFileName_);
        scriptFileName_ = "";
        unsavedChanges_ = false;
    }

    void PythonEditorWidget::clearOutput(){
        pythonOutput_->setText("");
    }

    void PythonEditorWidget::onTextChange(){
        unsavedChanges_ = true;
        const std::string source = pythonCode_->toPlainText().toLocal8Bit().constData();
        script_.setSource(source);
    }

    void PythonEditorWidget::startRecordingScript(){
        script_.getScriptRecorder()->startRecording();
        startRecordScriptButton_->setDisabled(true);
        endRecordScriptButton_->setDisabled(false);
    }

    void PythonEditorWidget::endRecordingScript(){
        script_.getScriptRecorder()->endRecording();
        startRecordScriptButton_->setDisabled(false);
        endRecordScriptButton_->setDisabled(true);
    }



} // namespace