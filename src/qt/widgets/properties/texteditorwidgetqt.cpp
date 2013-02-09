#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>


namespace inviwo {
TextEditorWidgetQt::TextEditorWidgetQt(Property* property,bool showProperty) : property_(property), btnProperty_("Edit","Open text editor"){
    btnWidget_ = new ButtonPropertyWidgetQt(&btnProperty_);
    showProperty_=showProperty;
    generateWidget();
    updateFromProperty();
}

void TextEditorWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    if (dynamic_cast<FileProperty*>(property_))
    {
        fileWidget_ = new FilePropertyWidgetQt(static_cast<FileProperty*>(property_));
        btnProperty_.registerClassMemberFunction(this, &TextEditorWidgetQt::editFile);
       
        if(showProperty_)
            hLayout->addWidget(fileWidget_);

    }
    else if (dynamic_cast<StringProperty*>(property_))
    {
        stringWidget_ = new StringPropertyWidgetQt(static_cast<StringProperty*>(property_));
        btnProperty_.registerClassMemberFunction(this, &TextEditorWidgetQt::editString);

        if(showProperty_) 
            hLayout->addWidget(stringWidget_);
    }

    hLayout->addWidget(btnWidget_);
    setLayout(hLayout);


    QVBoxLayout* textEditorLayout = new QVBoxLayout();
    textEditorLayout->setSpacing(0);
    textEditorLayout->setMargin(0);
    toolBar_ = new QToolBar();

    saveButton_ = new QToolButton();
    saveButton_->setIcon(QIcon(":/icons/network_save.png"));
 
    toolBar_->addWidget(saveButton_);
    toolBar_->addSeparator();

    mainWidget_ = new QWidget();
    textEditor_ = new QPlainTextEdit();
    textEditor_->createStandardContextMenu();

    textEditorLayout->addWidget(toolBar_);
    textEditorLayout->addWidget(textEditor_);
    mainWidget_->setLayout(textEditorLayout);



    //Using a toolbar instead of this
    //menuBar_ = new QMenuBar(mainWidget_);
    //fileMenu_ = new QMenu(tr("File"));
    //menuBar_->addMenu(fileMenu_);
    //fileMenu_->addAction(tr("Save"), this, SLOT(writeToFile()));

}

void TextEditorWidgetQt::setPropertyValue() {}


//Function loads the file into the textEditor_
void TextEditorWidgetQt::editFile(){
    connect(saveButton_, SIGNAL(pressed()), this, SLOT(writeToFile()));
    tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();
    textEditor_->setPlainText(QString::fromStdString(tmpPropertyValue_));
    file_ = new QFile(QString::fromStdString(tmpPropertyValue_));
    file_->open(QIODevice::ReadWrite);
    QTextStream textStream_(file_);
    mainWidget_->show();
    textEditor_->setPlainText(textStream_.readAll());   
}
//Function writes content of the textEditor_ to the file
void TextEditorWidgetQt::writeToFile(){
    //Close the file to open it with new flags
    file_->close();
    file_->open(QIODevice::WriteOnly|QIODevice::Truncate);
    QTextStream textStream_(file_);
    textStream_ << textEditor_->toPlainText();
    file_->close();
}
//Loads string into textEditor
void TextEditorWidgetQt::editString(){
    connect(saveButton_, SIGNAL(pressed()), this, SLOT(writeToString()));
    mainWidget_->show();
    tmpPropertyValue_ = static_cast<StringProperty*>(property_)->get();
    textEditor_->setPlainText(QString::fromStdString(tmpPropertyValue_));
}
void TextEditorWidgetQt::writeToString()
{
    static_cast<StringProperty*>(property_)->set(textEditor_->toPlainText().toStdString());
    stringWidget_->updateFromProperty();
}


void TextEditorWidgetQt::updateFromProperty() {}

} // namespace
