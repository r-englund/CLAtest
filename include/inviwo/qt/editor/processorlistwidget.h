#ifndef IVW_PROCESSORLISTWIDGET_H
#define IVW_PROCESSORLISTWIDGET_H

#include <QDockWidget>
#include <QListWidget>
#include <QMouseEvent>

namespace inviwo {

class ProcessorList : public QListWidget {

public:
    ProcessorList(QWidget* parent) : QListWidget(parent) {};
    ~ProcessorList() {};

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    QPoint dragStartPosition_;
};


class ProcessorListWidget : public QDockWidget {

public:
    ProcessorListWidget(QWidget* parent);
    ~ProcessorListWidget();

private:
    ProcessorList* processorList_;
    QPoint dragStartPosition_;
};


class ProcessorDragObject : public QDrag {
public:
    ProcessorDragObject(QWidget* source, const QString className);

    static bool canDecode(const QMimeData* mimeData);
    static bool decode(const QMimeData* mimeData, QString& className);
};

} // namespace

#endif // IVW_PROCESSORLISTWIDGET_H