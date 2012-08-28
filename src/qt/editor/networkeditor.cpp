
#include "inviwo/core/processors/canvasprocessor.h"

#include "inviwo/qt/editor/networkeditor.h"
#include "inviwo/qt/editor/processorlistwidget.h"
#include "inviwo/qt/editor/propertylistwidget.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>

namespace inviwo {

const std::string NetworkEditor::logSource_ = "NetworkEditor";

NetworkEditor* NetworkEditor::instance() {
    static NetworkEditor* newInstance = new NetworkEditor(qApp);
    return newInstance;
}

NetworkEditor::NetworkEditor(QObject* parent) : QGraphicsScene(parent) {
    connectionCurve_ = 0;
    startProcessor_ = 0;
    endProcessor_ = 0;
    setSceneRect(-1000,-1000,1000,1000);
    this->setBackgroundBrush(Qt::darkGray);

    processorNetwork_ = new ProcessorNetwork();
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);
}

NetworkEditor::~NetworkEditor() {
    // TODO: cleanup
}


void NetworkEditor::addConnection(ProcessorGraphicsItem* outProcessor, Port* outport,
                                  ProcessorGraphicsItem* inProcessor, Port* inport) {
    LogInfo("Adding connection.");
    // create connection in data flow network
    processorNetwork_->connectPorts(outport, inport);

    // generate GUI representation and add to editor
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(outProcessor, outport, inProcessor, inport);
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->show();
    processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    LogInfo("Removing connection.");
    // remove GUI representation from editor
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
    processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::addProcessor(std::string className, QPointF pos) {
    LogInfo("Adding processor.");
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

    // show property widgets
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->showProcessorProperties(processor);

    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor) {
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas());
    }
}

void NetworkEditor::removeProcessor(std::string identifier) {
    LogInfo("Removing processor.");
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

QGraphicsItem* NetworkEditor::getGraphicsItemAt(const QPointF pos) const {
    QGraphicsItem* graphicsItem = itemAt(pos);
    if (graphicsItem) {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItem);
        if (processorGraphicsItem)
            return processorGraphicsItem;
        else {
            ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItem);
            if (connectionGraphicsItem)
                return connectionGraphicsItem;
        }
    }
    return 0;
}

void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    startProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getGraphicsItemAt(e->scenePos()));
    if (startProcessor_) {
        startPort_ = startProcessor_->getSelectedPort(e->scenePos());
        if (startPort_ && startPort_->isOutport()) {
            QRectF portRect = startProcessor_->calculatePortRect(startPort_);
            portRect = startProcessor_->mapToScene(portRect).boundingRect();
            connectionCurve_ = new CurveGraphicsItem(portRect.center(), e->scenePos());
            connectionCurve_->setZValue(2.0);
            addItem(connectionCurve_);
            connectionCurve_->show();
        } else {
            LogInfo("Processor " + startProcessor_->getIdentifier() + " selected.");
            // show property widgets
            PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
            propertyListWidget_->showProcessorProperties(startProcessor_->getProcessor());
            QGraphicsScene::mousePressEvent(e);
        }
    } else
        QGraphicsScene::mousePressEvent(e);
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        connectionCurve_->setEndPoint(e->scenePos());
        connectionCurve_->update();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        removeItem(connectionCurve_);
        delete connectionCurve_;
        connectionCurve_ = 0;

        endProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getGraphicsItemAt(e->scenePos()));
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
    QGraphicsItem* graphicsItem = getGraphicsItemAt(e->scenePos());
    if (graphicsItem) {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItem);
        if (processorGraphicsItem) {
            QMenu menu;
            QAction* action = menu.addAction("Delete");
            QAction* result = menu.exec(QCursor::pos());
            if (result == action)
                removeProcessor(processorGraphicsItem->getIdentifier());
        } else {
            ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItem);
            if (connectionGraphicsItem) {
                QMenu menu;
                QAction* action = menu.addAction("Delete");
                QAction* result = menu.exec(QCursor::pos());
                if (result == action)
                    removeConnection(connectionGraphicsItem);
            }
        }
    } else
        QGraphicsScene::contextMenuEvent(e);
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

} // namespace