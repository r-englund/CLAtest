#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>


namespace inviwo {

ModifiedWidget::ModifiedWidget(){
    generateWidget();
}

void ModifiedWidget::closeEvent(QCloseEvent *event)
{
    bool test = true;
    if (mainParentWidget_->saveDialog()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ModifiedWidget::generateWidget(){

    QVBoxLayout* textEditorLayout = new QVBoxLayout();
    textEditorLayout->setSpacing(0);
    textEditorLayout->setMargin(0);
    toolBar_ = new QToolBar();

    saveButton_ = new QToolButton();
    saveButton_->setIcon(QIcon(":/icons/network_save.png"));
    saveButton_->setToolTip("Save file");
    unDoButton_ = new QToolButton();
    unDoButton_->setIcon(QIcon(":/icons/arrow_left.png"));
    unDoButton_->setToolTip("Undo");
    reDoButton_ = new QToolButton();
    reDoButton_->setIcon(QIcon(":/icons/arrow_right.png"));
    reDoButton_->setToolTip("Redo");
    reLoadButton_ = new QToolButton();
    reLoadButton_->setIcon(QIcon(":/icons/inviwo_tmp.png"));
    reLoadButton_->setToolTip("Reload");


    toolBar_->addWidget(saveButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(unDoButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(reDoButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(reLoadButton_);
    toolBar_->addSeparator();

    mainWidget_ = new QWidget();
    textEditor_ = new QPlainTextEdit();
    textEditor_->createStandardContextMenu();


    textEditorLayout->addWidget(toolBar_);
    textEditorLayout->addWidget(textEditor_);
    setLayout(textEditorLayout);

    connect(unDoButton_,SIGNAL(pressed()),textEditor_,SLOT(undo()));
    connect(reDoButton_,SIGNAL(pressed()),textEditor_,SLOT(redo()));
}

void ModifiedWidget::setParent(TextEditorWidgetQt* tmp){
    mainParentWidget_ = tmp;
}


TextEditorWidgetQt::TextEditorWidgetQt(Property* property) : property_(property), btnProperty_("Edit","Open text editor"){
    btnWidget_ = new ButtonPropertyWidgetQt(&btnProperty_);
    generateWidget();
    updateFromProperty();
}

void TextEditorWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    if (dynamic_cast<FileProperty*>(property_)) {

        QGridLayout* gridLayout = new QGridLayout();
        checkBox_ = new QCheckBox();
        fileWidget_ = new FilePropertyWidgetQt(static_cast<FileProperty*>(property_));
        btnProperty_.registerClassMemberFunction(this, &TextEditorWidgetQt::editFile);
        gridLayout->addWidget(fileWidget_,1,1,2,2);
        gridLayout->addWidget(new QLabel("Use system text editor"),3,1);
        gridLayout->addWidget(checkBox_,3,2);
        hLayout->addLayout(gridLayout);

    }
    else if (dynamic_cast<StringProperty*>(property_)) {
        stringWidget_ = new StringPropertyWidgetQt(static_cast<StringProperty*>(property_));
        btnProperty_.registerClassMemberFunction(this, &TextEditorWidgetQt::editString);
            hLayout->addWidget(stringWidget_);
    }

    hLayout->addWidget(btnWidget_);

    setLayout(hLayout);
     textEditorWidget_= new ModifiedWidget();
     textEditorWidget_->setParent(this);

}

void TextEditorWidgetQt::setPropertyValue() {}


//Function loads the file into the textEditor_
void TextEditorWidgetQt::editFile(){

    if (checkBox_->isChecked()) {
        if (static_cast<StringProperty*>(property_)->get() == "") {
            fileWidget_->setPropertyValue();
        }
        tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();
        const QString filePath_ = QString::fromStdString(tmpPropertyValue_);
        QUrl url_ = QUrl(filePath_);
        QDesktopServices::openUrl(url_);
    }
    else {
        if (static_cast<StringProperty*>(property_)->get() == "") {
            fileWidget_->setPropertyValue();
        }
        connect(textEditorWidget_->saveButton_, SIGNAL(pressed()), this, SLOT(writeToFile()));
        connect(textEditorWidget_->reLoadButton_, SIGNAL(pressed()), this, SLOT(loadFile()));
        loadFile();
        textEditorWidget_->show();
    }
}

void TextEditorWidgetQt::loadFile(){
    tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();
        file_ = new QFile(QString::fromStdString(tmpPropertyValue_));
        file_->open(QIODevice::ReadWrite);
        QTextStream textStream_(file_);
        textEditorWidget_->textEditor_->setPlainText(textStream_.readAll());
}

//Function writes content of the textEditor_ to the file
bool TextEditorWidgetQt::writeToFile(){
    //Close the file to open it with new flags
    file_->close();
    file_->open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream textStream_(file_);
    textStream_ << textEditorWidget_->textEditor_->toPlainText();
    file_->close();

    return true;
}
//Loads string into textEditor
void TextEditorWidgetQt::editString(){
    connect(textEditorWidget_->saveButton_, SIGNAL(pressed()), this, SLOT(writeToString()));
    connect(textEditorWidget_->reLoadButton_, SIGNAL(pressed()), this, SLOT(loadString()));
    loadString();
    textEditorWidget_->show();

}
void TextEditorWidgetQt::loadString(){
    tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();
    textEditorWidget_->textEditor_->setPlainText(QString::fromStdString(tmpPropertyValue_));
}
bool TextEditorWidgetQt::writeToString()
{
    static_cast<StringProperty*>(property_)->set(textEditorWidget_->textEditor_->toPlainText().toStdString());
    stringWidget_->updateFromProperty();
    return true;
}

bool TextEditorWidgetQt::saveDialog(){
    if (textEditorWidget_->textEditor_->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
            tr("The document has been modified.\n"
            "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save){
            if (dynamic_cast<FileProperty*>(property_))
                return TextEditorWidgetQt::writeToFile();   
            if (dynamic_cast<StringProperty*>(property_))
                return TextEditorWidgetQt::writeToString();
        }

        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}



void TextEditorWidgetQt::updateFromProperty() {}



} // namespace


