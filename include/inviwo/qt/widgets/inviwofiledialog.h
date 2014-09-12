#ifndef IVW_INVIWOFILEDIALOG_H
#define IVW_INVIWOFILEDIALOG_H

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <string>
#include <QFileDialog>
#include <QUrl>
#include <QDir>
#include <QSettings>
#include "qsettings.h"

namespace inviwo {

class IVW_QTWIDGETS_API InviwoFileDialog : public QFileDialog {
public:
    InviwoFileDialog(QWidget *parent, const std::string &title,
                     const std::string &pathType = "default");

    void addExtension(const std::string &ext, const std::string &description);
    void addExtension(const std::string &extString);

    void addSidebarPath(const InviwoApplication::PathType &path);
    void addSidebarPath(const std::string &path);
    void addSidebarPath(const QString &path);

    void useNativeDialog(const bool &use = true);

    virtual int exec();

    static QString getPreviousPath(const QString &pathType);
    static void setPreviousPath(const QString &pathType, const QString &path);

protected:
    QList<QUrl> sidebarURLs_;
    QStringList extension_;
    QString pathType_;

    static QSettings globalSettings_;
};
}  // namespace

#endif