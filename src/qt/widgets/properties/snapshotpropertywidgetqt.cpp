#include <inviwo/qt/widgets/properties/snapshotpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>

namespace inviwo {

SnapshotPropertyWidgetQt::SnapshotPropertyWidgetQt(SnapshotProperty *property) : property_(property){
    generateWidget();
    updateFromProperty();
}

void SnapshotPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    button_ = new QPushButton();
    button_->setText(QString::fromStdString(property_->getDisplayName()));
    connect(button_, SIGNAL(released()), this, SLOT(handleButton()));
    hLayout->addWidget(button_);
    setLayout(hLayout);
}

void SnapshotPropertyWidgetQt::handleButton(){
    // Set up and display the Save File dialog
    QStringList extension;
    extension << "All Files (*.*)";

    QString dataDir_ = QString::fromStdString(IVW_DIR+"data/");

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(dataDir_).absolutePath());
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));


    QFileDialog saveFileDialog(this, tr("Save File ..."), QDir(dataDir_).absolutePath());
    saveFileDialog.setFileMode(QFileDialog::AnyFile);
    
    if (saveFileDialog.exec()) {
        QString path = saveFileDialog.selectedFiles().at(0);
        // Save the path to the property
        property_->set(path.toLocal8Bit().constData());

        // Trigger snapshot creation
        property_->saveSnapshot();
    }
}
void SnapshotPropertyWidgetQt::updateFromProperty() {
}


} //namespace