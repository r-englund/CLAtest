#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>

namespace inviwo {

FilePropertyWidgetQt::FilePropertyWidgetQt(FileProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FilePropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    lineEdit_ = new QLineEdit();
    lineEdit_->setReadOnly(true);
    openButton_ = new QToolButton();
    openButton_->setIcon(QIcon(":/icons/network_open.png"));
    connect(openButton_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(lineEdit_);
    hLayout->addWidget(openButton_);
    setLayout(hLayout);
}

void FilePropertyWidgetQt::setPropertyValue() {
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
    updateFromProperty();
}

void FilePropertyWidgetQt::updateFromProperty() {
    lineEdit_->setText(QFileInfo(QString::fromStdString(property_->get())).fileName());
}


} // namespace
