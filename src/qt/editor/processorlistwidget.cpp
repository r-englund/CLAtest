#include <QApplication>

#include "inviwo/core/inviwoapplication.h"

#include "inviwo/qt/editor/processorlistwidget.h"

namespace inviwo {

ProcessorListWidget::ProcessorListWidget(QWidget* parent) : InviwoDockWidget(tr("Processors"), parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
   
    processorList_ = new ProcessorList(this);

    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::app();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<Processor*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            processorList_->addItem(new QListWidgetItem(QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
    }

    setWidget(processorList_);
}

ProcessorListWidget::~ProcessorListWidget() {}

void ProcessorList::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    QListWidget::mousePressEvent(e);
}

void ProcessorList::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QListWidgetItem* selectedProcessor = itemAt(dragStartPosition_);
        if (selectedProcessor)
            new ProcessorDragObject(this, selectedProcessor->text());
    }
}



static QString mimeType = "inviwo/ProcessorDragObject";

ProcessorDragObject::ProcessorDragObject(QWidget* source, const QString className) : QDrag(source) {
    QByteArray byteData;
    {
        QDataStream ds(&byteData, QIODevice::WriteOnly);
        ds << className;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType, byteData);
    mimeData->setData("text/plain", className.toAscii().data());
    setMimeData(mimeData);

    start(Qt::MoveAction);
}

bool ProcessorDragObject::canDecode(const QMimeData* mimeData) {
    if (mimeData->hasFormat(mimeType)) return true;
    else return false;
}

bool ProcessorDragObject::decode(const QMimeData* mimeData, QString& className) {
    QByteArray byteData = mimeData->data(mimeType);
    if (byteData.isEmpty())
        return false;

    QDataStream ds(&byteData, QIODevice::ReadOnly);
    ds >> className;
    return true;
}

} // namespace