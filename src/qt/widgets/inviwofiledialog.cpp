#include <inviwo/qt/widgets/inviwofiledialog.h>


#include <QUrl>
#include <QDir>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

namespace inviwo{

    InviwoFileDialog::InviwoFileDialog(QWidget * parent,const std::string &title, const QString &path)
        : QFileDialog(parent, title.c_str() , QDir(path).absolutePath())
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        sidebarURLs_ << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        sidebarURLs_ << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
        sidebarURLs_ << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
        sidebarURLs_ << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif
        useNativeDialog();
    }

    void InviwoFileDialog::useNativeDialog(const bool &use){
        QFileDialog::setOption(QFileDialog::DontUseNativeDialog,!use);
    }

    void InviwoFileDialog::addExtension(const std::string &ext,const std::string &description){
        std::stringstream ss;
        ss << description << "(*." << ext << ")";
        addExtension(ss.str());
    }

    void InviwoFileDialog::addExtension(const std::string &extString){
        extension_ << extString.c_str();
    }

    void InviwoFileDialog::addSidebarPath(const InviwoApplication::PathType &path){
        addSidebarPath(InviwoApplication::getPtr()->getPath(path));
    }

    void InviwoFileDialog::addSidebarPath(const std::string &path){
        sidebarURLs_ << QUrl::fromLocalFile(QDir(path.c_str()).absolutePath());
    }

    void InviwoFileDialog::addSidebarPath(const QString &path){
        sidebarURLs_ << QUrl::fromLocalFile(QDir(path).absolutePath());
    }
        
    int InviwoFileDialog::exec(){

        
        setNameFilters(extension_);
        setSidebarUrls(sidebarURLs_);
        return QFileDialog::exec();
    }
}