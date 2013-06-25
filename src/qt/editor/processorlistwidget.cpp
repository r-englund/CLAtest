#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QVBoxLayout>

#include <inviwo/core/common/inviwoapplication.h>

#include <inviwo/qt/editor/processorlistwidget.h>

namespace inviwo {

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

bool ProcessorListWidget::processorFits(ProcessorFactoryObject* processor, const QString& filter) {
    return (QString::fromStdString(processor->getClassName()).contains(filter, Qt::CaseInsensitive));
}

void ProcessorListWidget::addProcessorsToList(const QString& text) {
    processorList_->clear();
    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
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
    processorList_->sortItems(Qt::AscendingOrder);
}

void ProcessorTree::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    QTreeWidget::mousePressEvent(e);
}

void ProcessorTree::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QTreeWidgetItem* selectedProcessor = itemAt(dragStartPosition_);
        if (selectedProcessor)
            new ProcessorDragObject(this, selectedProcessor->text(0));
    }
}

ProcessorTreeWidget::ProcessorTreeWidget(QWidget* parent) : InviwoDockWidget(tr("Processors"), parent) {
    setObjectName("ProcessorTreeWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QFrame* frame = new QFrame();
    QVBoxLayout* vLayout = new QVBoxLayout(frame);

    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("Filter processor list...");
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(addProcessorsToTree(const QString&)));
    vLayout->addWidget(lineEdit);

    iconStable_ = QIcon(":/icons/processor_stable.png");
    iconExperimental_ = QIcon(":/icons/processor_experimental.png");
    iconBroken_ = QIcon(":/icons/processor_broken.png");

    processorTree_ = new ProcessorTree(this);
    processorTree_->setHeaderHidden(true);
    addProcessorsToTree();
    processorTree_->expandAll();
    vLayout->addWidget(processorTree_);

    frame->setLayout(vLayout);
    setWidget(frame);
}

ProcessorTreeWidget::~ProcessorTreeWidget() {}

bool ProcessorTreeWidget::processorFits(ProcessorFactoryObject* processor, const QString& filter) {
    return (QString::fromStdString(processor->getClassName()).contains(filter, Qt::CaseInsensitive));
}

QIcon* ProcessorTreeWidget::getCodeStateIcon(Processor::CodeState state){
    switch(state){
        case Processor::CODE_STATE_STABLE:
            return &iconStable_;
        case Processor::CODE_STATE_BROKEN:
            return &iconBroken_;
        default:
            return &iconExperimental_;
    }
}

void ProcessorTreeWidget::addProcessorsToTree(const QString& text) {
    processorTree_->clear();
    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
            if (text.isEmpty() || processorFits(curProcessorList[curProcessorId], text)) {
                QString categoryName = QString::fromStdString(curProcessorList[curProcessorId]->getCategory());
                QList<QTreeWidgetItem*> items = processorTree_->findItems(categoryName, Qt::MatchFixedString, 0);
                if(items.empty()){
                    items.push_back(new QTreeWidgetItem(QStringList(categoryName)));
                    processorTree_->addTopLevelItem(items[0]);
                }
                QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
                newItem->setIcon(0, *getCodeStateIcon(curProcessorList[curProcessorId]->getCodeState()));
                items[0]->addChild(newItem);
            }
        }
    }
    processorTree_->sortItems(0, Qt::AscendingOrder);
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