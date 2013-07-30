#ifndef IVW_HTMLLISTWIDGETQT_H
#define IVW_HTMLLISTWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>
#include <QTreeWidget>
#include <QDrag>

namespace inviwo {

class IVW_QTWIDGETS_API HtmlTree : public QTreeWidget {

public:
    HtmlTree(QWidget* parent) : QTreeWidget(parent) { setObjectName("htmlTree");};
    ~HtmlTree() {};

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    QPoint dragStartPosition_;
};

class IVW_QTWIDGETS_API HtmlTreeWidget : public QWidget {
    Q_OBJECT
public:
    HtmlTreeWidget(QWidget* parent=0);
    ~HtmlTreeWidget();

private:
    HtmlTree* processorTree_;
    QPoint dragStartPosition_;

    bool processorFits(ProcessorFactoryObject* processor, const QString& filter);
    //QIcon* getCodeStateIcon(Processor::CodeState);

private slots:
    void addTagsToTree(const QString& text="");

private:
    //QIcon iconStable_;
    //QIcon iconExperimental_;
    //QIcon iconBroken_;
};

class IVW_QTWIDGETS_API HtmlDragObject : public QDrag {
public:
    HtmlDragObject(QWidget* source, const QString className);

    static bool canDecode(const QMimeData* mimeData);
    static bool decode(const QMimeData* mimeData, QString& className);
};

} // namespace

#endif // IVW_HTMLLISTWIDGETQT_H