#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

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
    openButton_->setIcon(QIcon(":/icons/open.png"));
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

    //TODO: create InviwoFileDialog to avoid frequent version checks
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif
    
    QFileDialog openFileDialog(this, tr("Open File ..."), QDir(dataDir_).absolutePath());
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setNameFilters(extension);
    openFileDialog.setSidebarUrls(sidebarURLs);

    if (openFileDialog.exec()) {
        QString path = openFileDialog.selectedFiles().at(0);
        property_->set(path.toLocal8Bit().constData());
    }
    updateFromProperty();
    emit modified();
}

void FilePropertyWidgetQt::updateFromProperty() {
    lineEdit_->setText(QFileInfo(QString::fromStdString(property_->get())).fileName());
}


} // namespace
