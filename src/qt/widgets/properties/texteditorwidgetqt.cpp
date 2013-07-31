#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

ModifiedWidget::ModifiedWidget(){
    generateWidget();
}

void ModifiedWidget::closeEvent(QCloseEvent *event)
{
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
    saveButton_->setIcon(QIcon(":/icons/save.png")); // Temporary icon
    saveButton_->setToolTip("Save file");
    unDoButton_ = new QToolButton();
    unDoButton_->setIcon(QIcon(":/icons/arrow_left.png")); // Temporary icon
    unDoButton_->setToolTip("Undo");
    reDoButton_ = new QToolButton();
    reDoButton_->setIcon(QIcon(":/icons/arrow_right.png")); // Temporary icon
    reDoButton_->setToolTip("Redo");
    reLoadButton_ = new QToolButton();
    reLoadButton_->setIcon(QIcon(":/icons/inviwo_tmp.png")); // Temporary icon
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


TextEditorWidgetQt::TextEditorWidgetQt(Property* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void TextEditorWidgetQt::generateWidget() {

    QHBoxLayout* hLayout = new QHBoxLayout();
    btnEdit_ = new QToolButton();
    btnEdit_->setIcon(QIcon(":/icons/edit.png"));

    if (dynamic_cast<FileProperty*>(property_)) {

        fileWidget_ = new FilePropertyWidgetQt(static_cast<FileProperty*>(property_));
        connect(btnEdit_,SIGNAL(clicked()),this,SLOT(editFile()));
        hLayout->addWidget(fileWidget_);

    }
    else if (dynamic_cast<StringProperty*>(property_)) {

        stringWidget_ = new StringPropertyWidgetQt(static_cast<StringProperty*>(property_));
        connect(btnEdit_,SIGNAL(clicked()),this,SLOT(editString()));
        hLayout->addWidget(stringWidget_);
    }
    hLayout->addWidget(btnEdit_);
    setLayout(hLayout);
    hLayout->setContentsMargins(QMargins(0,0,0,0));

    textEditorWidget_= new ModifiedWidget();
    textEditorWidget_->setParent(this);

    htmlEditorWidget_ = new HtmlEditorWidgetQt();
    htmlEditorWidget_->setParent(this);
}

void TextEditorWidgetQt::setPropertyValue() {}

//Function loads the file into the textEditor_
void TextEditorWidgetQt::editFile(){

     // fetch settings from the settings menu to determine what editor to use
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    
    if (dynamic_cast<BoolProperty*>(inviwoApp->getSettings()->getPropertyByIdentifier("txtEditor"))->get()) { 
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

        connect(htmlEditorWidget_->saveButton_, SIGNAL(pressed()), this, SLOT(writeToFile()));
        connect(htmlEditorWidget_->reLoadButton_, SIGNAL(pressed()), this, SLOT(loadFile()));

        loadFile();

        std::string fileName = static_cast<StringProperty*>(property_)->get();
        std::string extension = UrlParser::getFileExtension(fileName);
        if (extension=="html" || extension=="htm")
            htmlEditorWidget_->show();
        else
            textEditorWidget_->show();
    }
}

void TextEditorWidgetQt::loadFile(){
    tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();

    file_ = new QFile(QString::fromStdString(tmpPropertyValue_));
    file_->open(QIODevice::ReadWrite);
    QTextStream textStream_(file_);

    std::string extension = UrlParser::getFileExtension(tmpPropertyValue_);

    if (extension == "html" || extension == "htm") {
        htmlEditorWidget_->htmlEditor_->setPlainText(textStream_.readAll());
    }
    else {
        textEditorWidget_->textEditor_->setPlainText(textStream_.readAll());
    }   
    
}

//Function writes content of the textEditor_ to the file
bool TextEditorWidgetQt::writeToFile(){
    //Close the file to open it with new flags
    file_->close();
    file_->open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream textStream(file_);

    QFileInfo qfileInfo(file_->fileName());
    QString qfilename(qfileInfo.fileName());

    std::string fileName = qfilename.toStdString();
    std::string extension = UrlParser::getFileExtension(fileName);

    if (extension == "html" || extension == "htm") {
        textStream <<  htmlEditorWidget_->htmlOutput_->toPlainText();
    }
    else {
        textStream << textEditorWidget_->textEditor_->toPlainText();
    }
    
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
bool TextEditorWidgetQt::writeToString() {
    static_cast<StringProperty*>(property_)->set(textEditorWidget_->textEditor_->toPlainText().toLocal8Bit().constData());
    stringWidget_->updateFromProperty();
    return true;
}

bool TextEditorWidgetQt::saveDialog(){
    if (textEditorWidget_->textEditor_->document()->isModified() ||
        htmlEditorWidget_->htmlEditor_->document()->isModified()) {
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



void TextEditorWidgetQt::updateFromProperty() {
    StringProperty* stringProp = dynamic_cast<StringProperty*>(property_);
    FileProperty* fileProp = dynamic_cast<FileProperty*>(property_);
    if (stringProp)
       stringWidget_->updateFromProperty();
    else if (fileProp)
        fileWidget_->updateFromProperty();
}



} // namespace


