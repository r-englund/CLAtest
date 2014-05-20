#ifndef IVW_INVIWOFILEDIALOG_H
#define IVW_INVIWOFILEDIALOG_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <string>
#include <QFileDialog>
#include <QUrl>
#include <QDir>

namespace inviwo {

    class IVW_QTWIDGETS_API InviwoFileDialog : public QFileDialog{
    public:
        InviwoFileDialog(QWidget * parent,const std::string &title, const QString &path = QString(""));
        //InviwoFileDialog(QWidget * parent,const std::string &title, const std::string &path);

        void addExtension(const std::string &ext,const std::string &description);
        void addExtension(const std::string &extString);

        void addSidebarPath(const InviwoApplication::PathType &path);
        void addSidebarPath(const std::string &path);
        void addSidebarPath(const QString &path);

        void useNativeDialog(const bool &use = true);

        virtual int exec();


    protected:
        QList<QUrl> sidebarURLs_;
        QStringList extension_;
    };
}  // namespace

#endif