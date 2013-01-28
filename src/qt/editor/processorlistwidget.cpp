#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QVBoxLayout>

#include <inviwo/core/inviwoapplication.h>

#include <inviwo/qt/editor/processorlistwidget.h>

namespace inviwo {

ProcessorListWidget::ProcessorListWidget(QWidget* parent) : InviwoDockWidget(tr("Processors"), parent) {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QFrame* frame = new QFrame();
    QVBoxLayout* vLayout = new QVBoxLayout(frame);
   
    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("Filter processor list...");
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(addProcessorsToList(const QString&)));
    vLayout->addWidget(lineEdit);

    processorList_ = new ProcessorList(this);
    addProcessorsToList();
    vLayout->addWidget(processorList_);

    frame->setLayout(vLayout);
    setWidget(frame);
}

ProcessorListWidget::~ProcessorListWidget() {}

bool ProcessorListWidget::processorFits(Processor* processor, const QString& filter) {
    return (QString::fromStdString(processor->getClassName()).contains(filter, Qt::CaseInsensitive));
}

void ProcessorListWidget::addProcessorsToList(const QString& text) {
    processorList_->clear();
    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<Processor*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
            if (text.isEmpty() || processorFits(curProcessorList[curProcessorId], text)) {
                QString iconName = ":/icons/processor_experimental.png";
                if (curProcessorList[curProcessorId]->getCodeState() == Processor::CODE_STATE_BROKEN)
                    iconName = ":/icons/processor_broken.png";
                else if (curProcessorList[curProcessorId]->getCodeState() == Processor::CODE_STATE_STABLE)
                    iconName = ":/icons/processor_stable.png";
                processorList_->addItem(new QListWidgetItem(QIcon(iconName),
                                        QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
            }
        }
    }
}


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