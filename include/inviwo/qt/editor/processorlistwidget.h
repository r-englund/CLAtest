#ifndef IVW_PROCESSORLISTWIDGET_H
#define IVW_PROCESSORLISTWIDGET_H

#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>

#include "inviwo/qt/editor/inviwodockwidget.h"

#include "inviwo/core/processors/processor.h"

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


class ProcessorListWidget : public InviwoDockWidget {
Q_OBJECT
public:
    ProcessorListWidget(QWidget* parent);
    ~ProcessorListWidget();

private:
    ProcessorList* processorList_;
    QPoint dragStartPosition_;

    bool processorFits(Processor* processor, const QString& filter);

private slots:
    void addProcessorsToList(const QString& text="");
};


class ProcessorDragObject : public QDrag {
public:
    ProcessorDragObject(QWidget* source, const QString className);

    static bool canDecode(const QMimeData* mimeData);
    static bool decode(const QMimeData* mimeData, QString& className);
};

} // namespace

#endif // IVW_PROCESSORLISTWIDGET_H