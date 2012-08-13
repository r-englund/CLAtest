
#include "inviwo/core/processors/canvasprocessor.h"

#include "inviwo/qt/editor/networkeditor.h"
#include "inviwo/qt/editor/processorlistwidget.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

namespace inviwo {

NetworkEditor* NetworkEditor::instance() {
    static NetworkEditor* newInstance = new NetworkEditor(qApp);
    return newInstance;
}

NetworkEditor::NetworkEditor(QObject* parent) : QGraphicsScene(parent) {
    connectionLine_ = 0;
    startProcessor_ = 0;
    endProcessor_ = 0;
    setSceneRect(-1000, -1000, 1000, 1000);

    processorNetwork_ = new ProcessorNetwork();
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);
}

NetworkEditor::~NetworkEditor() {
    // TODO: cleanup
}


void NetworkEditor::addConnection(ProcessorGraphicsItem* outProcessor, Port* outport, ProcessorGraphicsItem* inProcessor, Port* inport) {
    // create connection in data flow network
    processorNetwork_->connectPorts(outport, inport);

    // generate GUI representation and add to editor
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(outProcessor, outport, inProcessor, inport);
    QPointF startPoint = outProcessor->mapToScene(outProcessor->calculatePortRect(outport)).boundingRect().center();
    QPointF endPoint = inProcessor->mapToScene(inProcessor->calculatePortRect(inport)).boundingRect().center();
    connectionGraphicsItem->setLine(startPoint.x(), startPoint.y(), endPoint.x(), endPoint.y());
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->show();
    processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    // remove GUI representation from editor
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
    processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::addProcessor(std::string className, QPointF pos) {
    // create processor and add to data flow network
    Processor* processor = ProcessorFactory::instance()->create(className);
    // TODO: if identifier classname exists, generate a new identifier
    processor->setIdentifier(className);
    processor->createProcessorWidget();
    processorNetwork_->addProcessor(processor);

    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem();
    processorGraphicsItem->setProcessor(processor);
    // TODO: if (!sceneRect().contains(pos)) CLAMP_TO_SCENE_RECT;
    processorGraphicsItem->setPos(pos);
    processorGraphicsItems_.push_back(processorGraphicsItem);
    addItem(processorGraphicsItem);
    processorGraphicsItem->show();

    // show processor widget
    if (processor->hasProcessorWidget()) {
        processor->getProcessorWidget()->initialize();
        processor->getProcessorWidget()->show();
    }

    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor) {
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas());

    }
}

void NetworkEditor::removeProcessor(std::string identifier) {
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(identifier);

    // remove processor from data flow network and delete it
    Processor* processor = processorGraphicsItem->getProcessor();
    processorNetwork_->removeProcessor(processor);
    processorGraphicsItem->setProcessor(0);
    delete processor;

    // remove GUI connections
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::instance()->connectionGraphicsItems_;
    for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
        if (connectionGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            connectionGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
            removeConnection(connectionGraphicsItems[i]);
        }
    }

    // remove GUI representation from editor
    processorGraphicsItem->hide();
    removeItem(processorGraphicsItem);
    processorGraphicsItems_.erase(std::remove(processorGraphicsItems_.begin(), processorGraphicsItems_.end(),
                                  processorGraphicsItem), processorGraphicsItems_.end());

    delete processorGraphicsItem;
    processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItem(std::string identifier) const {
    for (size_t i=0; i<processorGraphicsItems_.size(); i++)
        if (processorGraphicsItems_[i]->getIdentifier() == identifier)
            return processorGraphicsItems_[i];
    return 0;
}

ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItem(const QPointF pos) const {
    QGraphicsItem* graphicsItem = itemAt(pos);
    if (graphicsItem) {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItem);
        if (processorGraphicsItem)
            return processorGraphicsItem;
    }
    return 0;
}





ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItem(unsigned int index) const {
    if (index > 0 && index <= connectionGraphicsItems_.size())
        return connectionGraphicsItems_[index];
    return 0;
}

ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItem(const QString name) const {
    /*
    for(unsigned int i=0; i<connectionGraphicsItems_.size(); i++)
        if(connectionGraphicsItems_[i]->name() == name)
            return connectionGraphicsItems_[i];
    */
    return 0;
}

/*
QList<CWrapperLink*> NetworkEditor::link(CWrapper* output, CWrapper* input)
{
    QList<CWrapperLink*> ret;

    for(int i=0; i<d->wrapperLinkList.count(); i++)
    {
        CWrapperLink* link = d->wrapperLinkList[i];
        if(link->input() == input && link->output() == output)
            ret.push_back(link);
    }

    return ret;
}
*/

void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    startProcessor_ = getProcessorGraphicsItem(e->scenePos());
    if (startProcessor_) {
        startPort_ = startProcessor_->getSelectedPort(e->scenePos());
        if (startPort_ && startPort_->isOutport()) {
            connectionLine_ = new QGraphicsLineItem(0, this);
            connectionLine_->setZValue(3.0f);
            connectionLine_->setPen(QPen(Qt::darkGray, 4.0f));
            QRectF portRect = startProcessor_->calculatePortRect(startPort_);
            portRect = startProcessor_->mapToScene(portRect).boundingRect();
            connectionLine_->setLine(portRect.center().x(), portRect.center().y(), e->scenePos().x(), e->scenePos().y());
            connectionLine_->show();
        } else
            QGraphicsScene::mousePressEvent(e);
    }
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionLine_) {
        QLineF line = connectionLine_->line();
        connectionLine_->setLine(line.x1(), line.y1(), e->scenePos().x(), e->scenePos().y());
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionLine_) {
        delete connectionLine_;
        connectionLine_ = 0;

        endProcessor_ = getProcessorGraphicsItem(e->scenePos());
        if (endProcessor_) {
            endPort_ = endProcessor_->getSelectedPort(e->scenePos());
            if (endPort_ && !endPort_->isOutport())
                addConnection(startProcessor_, startPort_, endProcessor_, endPort_);
        }
        startProcessor_ = 0; 
        endProcessor_ = 0;
    } else
        QGraphicsScene::mouseReleaseEvent(e);
}

void NetworkEditor::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    ProcessorGraphicsItem* w = getProcessorGraphicsItem(e->scenePos());
    if (!w) {
        QGraphicsScene::contextMenuEvent(e);
        return;
    }
    QMenu menu;
    QAction* action = menu.addAction("Delete");
    QAction* result = menu.exec(QCursor::pos());
    if (result == action) {
        removeProcessor(w->getIdentifier());
    }
}


// drag and drop of processors
void NetworkEditor::dragEnterEvent(QGraphicsSceneDragDropEvent* e) {
    dragMoveEvent(e);
}

void NetworkEditor::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (ProcessorDragObject::canDecode(e->mimeData())) {
        e->setAccepted(true);
        e->acceptProposedAction();
    }
}

void NetworkEditor::dropEvent(QGraphicsSceneDragDropEvent* e) {
    if (ProcessorDragObject::canDecode(e->mimeData())) {
        QString className;
        ProcessorDragObject::decode(e->mimeData(), className);
        if (!className.isEmpty()) {
            addProcessor(className.toStdString(), e->scenePos());
            e->setAccepted(true);
            e->acceptProposedAction();
        }
    }
}


/*
void NetworkEditor::addConnection(ConnectionGraphicsItem* conncetionGraphicsItem) {
    if (!conncetionGraphicsItem) return;
    connectionGraphicsItems_.push_back(conncetionGraphicsItem);
    conncetionGraphicsItem->show();
}

void NetworkEditor::removeConnection(ConnectionGraphicsItem* conncetionGraphicsItem) {
    if (!conncetionGraphicsItem) return;
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
                                               conncetionGraphicsItem), connectionGraphicsItems_.end());
}
*/

} // namespace