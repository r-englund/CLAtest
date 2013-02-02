#include <inviwo/qt/widgets/properties/texteditorwidgetqt.h>

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>
#include <QFile>
#include <QTextStream>

namespace inviwo {

TextEditorWidgetQt::TextEditorWidgetQt(TextEditorProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void TextEditorWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    textEditor_ = new QPlainTextEdit();
    //Adds contextMenu
    textEditor_->createStandardContextMenu();
    lineEdit_ = new QLineEdit();
    lineEdit_->setReadOnly(true);
    editButton_ = new QPushButton();
    editButton_->setText("Edit File");
    openButton_ = new QToolButton();
    openButton_->setIcon(QIcon(":/icons/network_open.png"));
    connect(openButton_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
    connect(editButton_,SIGNAL(pressed()),this,SLOT(editFile()));
    hLayout->addWidget(lineEdit_);
    hLayout->addWidget(openButton_);
    hLayout->addWidget(editButton_);
    setLayout(hLayout);
}

void TextEditorWidgetQt::setPropertyValue() {
    // dialog window settings
    QStringList extension;
    extension << "All Files (*.*)";

    QString dataDir_ = QString::fromStdString(IVW_DIR+"data/");

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(dataDir_).absolutePath());
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));

    QFileDialog openFileDialog(this, tr("Open File ..."), QDir(dataDir_).absolutePath());
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilters(extension);
    openFileDialog.setSidebarUrls(sidebarURLs);

    if (openFileDialog.exec()) {
        QString path = openFileDialog.selectedFiles().at(0);
        property_->set(path.toStdString());
    }

}
//Function loads the file into the textEditor_
void TextEditorWidgetQt::editFile(){
    file_ = new QFile(QString::fromStdString(property_->get()));
    file_->open(QIODevice::ReadWrite);
    QTextStream textStream_(file_);
    textEditor_->show();
    textEditor_->setPlainText(textStream_.readAll());
    connect(textEditor_,SIGNAL(textChanged ()),this,SLOT(writeToFile()));
   
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

void TextEditorWidgetQt::updateFromProperty() {
    lineEdit_->setText(QFileInfo(QString::fromStdString(property_->get())).fileName());
}

} // namespace
