
#include "inviwo/core/processors/canvasprocessor.h"
#include "inviwo/core/inviwofactorybase.h"
#include "inviwo/core/io/serialization/ivwserializer.h"
#include "inviwo/core/io/serialization/ivwdeserializer.h"
#include "inviwo/qt/editor/networkeditor.h"
#include "inviwo/qt/editor/processorlistwidget.h"
#include "inviwo/qt/editor/propertylistwidget.h"
#include "inviwo/core/metadata/positionmetadata.h"

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

void NetworkEditor::createDefaultRenderContext(QWidget *qwidget) {         
    defaultRenderContext_ = new CanvasQt(qwidget);
    defaultRenderContext_->setNetworkEvaluator(0) ;
    defaultRenderContext_->switchContext();
    processorNetworkEvaluator_->setDefaultRenderContext(defaultRenderContext_) ;
}

NetworkEditor::~NetworkEditor() {
    // TODO: cleanup
}

void NetworkEditor::addConnection(PortConnection *connection) {
    ProcessorGraphicsItem* outProcessorItem=0;
    ProcessorGraphicsItem* inProcessorItem=0;

    Processor* outProcessor =  connection->getOutport()->getProcessor();
    Processor* inProcessor =  connection->getInport()->getProcessor();

    outProcessorItem = getProcessorGraphicsItem(outProcessor->getIdentifier());
    inProcessorItem = getProcessorGraphicsItem(inProcessor->getIdentifier());

    initializeConnectionRepresentation(outProcessorItem, connection->getOutport(), inProcessorItem, connection->getInport()) ;
}

void NetworkEditor::initializeConnectionRepresentation(ProcessorGraphicsItem* outProcessor, Port* outport,
                                                       ProcessorGraphicsItem* inProcessor, Port* inport) {

    // generate GUI representation and add to editor
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(outProcessor, outport, inProcessor, inport);
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->show();
    //processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::addConnection(ProcessorGraphicsItem* outProcessor, Port* outport,
                                  ProcessorGraphicsItem* inProcessor, Port* inport) {
    LogInfo("Adding connection.");
    // create connection in data flow network
    processorNetwork_->connectPorts(outport, inport);

    initializeConnectionRepresentation(outProcessor, outport, inProcessor, inport) ;
}

void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    LogInfo("Removing connection.");
    // remove GUI representation from editor
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
    //processorNetworkEvaluator_->initializeNetwork();
    processorNetworkEvaluator_->evaluate();
}

bool NetworkEditor::processorWithIdentifierExists(std::string identifier) {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        if (processors[i]->getIdentifier() == identifier)
            return true;
    return false;
}

Processor* NetworkEditor::createProcessor(std::string className) {
    LogInfo("Adding processor.");
    // create processor and add to data flow network    
    Processor* processor = dynamic_cast<Processor*>(InviwoFactoryBase::instance<ProcessorFactory>()->create(className));

    // if identifier classname already exists, generate a new identifier
    std::string identifier = className;
    if (processorWithIdentifierExists(identifier)) {
        unsigned int idNumber = 1;
        do {
            std::stringstream stringStream;
            stringStream << idNumber++;            
            identifier = className + stringStream.str();
        } while (processorWithIdentifierExists(identifier));
    }

    processor->setIdentifier(identifier);
    processorNetwork_->addProcessor(processor);

    return processor;
}

void NetworkEditor::initializeProcessorRepresentation(Processor* processor, QPointF pos) {
    processor->createProcessorWidget();

    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem();
    processorGraphicsItem->setProcessor(processor);
    // TODO: if (!sceneRect().contains(pos)) CLAMP_TO_SCENE_RECT;
    processorGraphicsItem->setPos(pos);
    processorGraphicsItem->updateMetaData();
    processorGraphicsItems_.push_back(processorGraphicsItem);
    addItem(processorGraphicsItem);
    processorGraphicsItem->show();
    processor->initialize();

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
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());
    }
}

void NetworkEditor::removeProcessor(std::string identifier) {
    LogInfo("Removing processor.");
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(identifier);

    // remove processor from data flow network and delete it    
    Processor* processor = processorGraphicsItem->getProcessor();

    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor) {
        processorNetworkEvaluator_->deRegisterCanvas(canvasProcessor->getCanvas());
    }    

    processorNetwork_->removeProcessor(processor);
    processorGraphicsItem->setProcessor(0);
    processor->deinitialize();
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
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::clearNetwork() {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
     for (size_t i = 0; i < processors.size(); ++i) {
        removeProcessor(processors[i]->getIdentifier()) ;
    }
}

ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItem(std::string identifier) const {
    for (size_t i=0; i<processorGraphicsItems_.size(); i++)
        if (processorGraphicsItems_[i]->getIdentifier() == identifier)
            return processorGraphicsItems_[i];
    return 0;
}

QGraphicsItem* NetworkEditor::getProcessorGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    if (graphicsItems.size() > 0) {
        for (int i=0; i<graphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItems[i]);
            if (processorGraphicsItem)
                return processorGraphicsItem;
        }
    }  
    return 0;
}

QGraphicsItem* NetworkEditor::getConnectionGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    if (graphicsItems.size() > 0) {
        for (int i=0; i<graphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
            if (connectionGraphicsItem)
                return connectionGraphicsItem;
        }
    }  
    return 0;
}

void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    startProcessor_ = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
    if (startProcessor_) {
        startPort_ = startProcessor_->getSelectedPort(e->scenePos());
        if (startPort_ && startPort_->isOutport()) {
            QRectF portRect = startProcessor_->calculatePortRect(startPort_);
            portRect = startProcessor_->mapToScene(portRect).boundingRect();
            connectionCurve_ = new CurveGraphicsItem(portRect.center(), e->scenePos());
            connectionCurve_->setZValue(2.0);
            addItem(connectionCurve_);
            connectionCurve_->show();
        } else if (startPort_ && startPort_->isInport()) {
            if (startPort_->isConnected()) {
                std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
                for (size_t i=0; i<portConnections.size(); i++) {
                    const Port* curInport = portConnections[i]->getInport();
                    if (curInport == startPort_) {
                        Port* connectedOutport = portConnections[i]->getOutport();
                        startPort_->disconnectFrom(connectedOutport);
                        startPort_ = connectedOutport;
                    }
                }
                ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(getConnectionGraphicsItemAt(e->scenePos()));
                QPointF startPoint = connectionGraphicsItem->getStartPoint();
                startProcessor_ = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(startPoint));
                removeConnection(connectionGraphicsItem);
                connectionCurve_ = new CurveGraphicsItem(startPoint, e->scenePos());
                connectionCurve_->setZValue(2.0);
                addItem(connectionCurve_);
                connectionCurve_->show();
            }
        } else {
            LogInfo("Processor " + startProcessor_->getIdentifier() + " selected.");
            // show property widgets
            PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
            propertyListWidget_->showProcessorProperties(startProcessor_->getProcessor());
            QGraphicsScene::mousePressEvent(e);
        }
        e->accept();
    } else
        QGraphicsScene::mousePressEvent(e);
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        connectionCurve_->setEndPoint(e->scenePos());
        connectionCurve_->update();
        e->accept();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        removeItem(connectionCurve_);
        delete connectionCurve_;
        connectionCurve_ = 0;

        endProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (endProcessor_) {
            endPort_ = endProcessor_->getSelectedPort(e->scenePos());
            if (endPort_ && !endPort_->isOutport())
                addConnection(startProcessor_, startPort_, endProcessor_, endPort_);
        }
        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();
    } else
        QGraphicsScene::mouseReleaseEvent(e);
}

void NetworkEditor::keyPressEvent(QKeyEvent* keyEvent) {
    // delete selected graphics items
    if (keyEvent->key() == Qt::Key_Delete) {
        // first delete connections
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem = dynamic_cast<ConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (connectionGraphicsItem)
                removeConnection(connectionGraphicsItem);
        }
        // second delete processors
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
            if (processorGraphicsItem)
                removeProcessor(processorGraphicsItem->getIdentifier());
        }
    }
}

void NetworkEditor::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
    ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(getConnectionGraphicsItemAt(e->scenePos()));
    if (processorGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == action)
            removeProcessor(processorGraphicsItem->getIdentifier());
    } else if (connectionGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == action)
            removeConnection(connectionGraphicsItem);
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
            Processor* processor = createProcessor(className.toStdString());
            initializeProcessorRepresentation(processor, e->scenePos());
            e->setAccepted(true);
            e->acceptProposedAction();
        }
    }
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    IvwSerializer xmlSerializer(fileName);
    //xmlSerializer.serialize("NetworkEditor", *this) ;
    processorNetwork_->serialize(xmlSerializer);

    xmlSerializer.writeFile(std::cout);
    
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {

    clearNetwork();

    IvwDeserializer xmlDeserializer(fileName);

    processorNetwork_->deserialize(xmlDeserializer);

    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    for (std::vector<Processor*>::iterator it = processors.begin(); it!=processors.end(); it++) {
        Processor* p = *it;
        PositionMetaData* meta = dynamic_cast<PositionMetaData*>(p->getMetaData("PositionMetaData"));        
        initializeProcessorRepresentation(p, QPointF(meta->getX(), meta->getY()));
    }

    std::vector<PortConnection*> connections = processorNetwork_->getPortConnections();

    for (std::vector<PortConnection*>::iterator it = connections.begin(); it!=connections.end(); it++) {
       addConnection(*it) ;
    }
    
    return true;
}

} // namespace