
#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/metadata/positionmetadata.h>
#include <inviwo/core/util/inviwofactorybase.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>

namespace inviwo {

const std::string NetworkEditor::logSource_ = "NetworkEditor";
NetworkEditor* NetworkEditor::instance_ = 0;

NetworkEditor* NetworkEditor::instance() {
    if (!instance_)
        instance_ = new NetworkEditor(qApp);
    return instance_;
}

NetworkEditor::NetworkEditor(QObject* parent) : QGraphicsScene(parent) {
    connectionCurve_ = 0;
    linkCurve_ = 0;
    startProcessor_ = 0;
    endProcessor_ = 0;
    setSceneRect(-1000,-1000,1000,1000);
    gridSnapping_ = true;

    processorNetwork_ = new ProcessorNetwork();
    InviwoApplication::getRef().setProcessorNetwork(processorNetwork_);
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);
}

ProcessorNetworkEvaluator* NetworkEditor::getProcessorNetworkEvaluator() {         
    return processorNetworkEvaluator_;
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
    processorNetworkEvaluator_->evaluate();    
}

void NetworkEditor::initializeLinkRepresentation(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor) {

    LinkConnectionGraphicsItem* linkGraphicsItem = new LinkConnectionGraphicsItem(outProcessor, inProcessor);
    linkGraphicsItems_.push_back(linkGraphicsItem);
    addItem(linkGraphicsItem);
    linkGraphicsItem->show();
    showLinkDialog(linkGraphicsItem);
}

void NetworkEditor::addConnection(ProcessorGraphicsItem* outProcessor, Port* outport,
                                  ProcessorGraphicsItem* inProcessor, Port* inport) {
    LogInfo("Adding connection.");
    // create connection in data flow network
    processorNetwork_->connectPorts(outport, inport);

    initializeConnectionRepresentation(outProcessor, outport, inProcessor, inport);
}

void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    LogInfo("Removing connection.");
    // remove GUI representation from editor
    processorNetwork_->disconnectPorts(connectionGraphicsItem->getInport(), connectionGraphicsItem->getOutport());
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::addLink(ProcessorLink *link) {
    ProcessorGraphicsItem* outProcessorItem=0;
    ProcessorGraphicsItem* inProcessorItem=0;

    Processor* outProcessor =  link->getInProcessor();
    Processor* inProcessor =  link->getOutProcessor();

    outProcessorItem = getProcessorGraphicsItem(outProcessor->getIdentifier());
    inProcessorItem = getProcessorGraphicsItem(inProcessor->getIdentifier());

    initializeLinkRepresentation(outProcessorItem, inProcessorItem);
}

void NetworkEditor::addLink(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor) {
    LogInfo("Adding link.");
    // create link in data flow network
    processorNetwork_->addLink(outProcessor->getProcessor(), inProcessor->getProcessor());
    initializeLinkRepresentation(outProcessor, inProcessor);
}

void NetworkEditor::removeLink(LinkConnectionGraphicsItem* linkGraphicsItem) {
    LogInfo("Removing link.");
    // remove GUI representation from editor
    processorNetwork_->removeLink(linkGraphicsItem->getOutProcessor()->getProcessor(), linkGraphicsItem->getInProcessor()->getProcessor());
    linkGraphicsItem->hide();
    removeItem(linkGraphicsItem);
    linkGraphicsItems_.erase(std::remove(linkGraphicsItems_.begin(), linkGraphicsItems_.end(), linkGraphicsItem),
                             linkGraphicsItems_.end());
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
    Processor* processor = dynamic_cast<Processor*>(ProcessorFactory::getRef().create(className));

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

QPointF NetworkEditor::snapToGrid(QPointF pos) {
    int gridInterval = 30;
    QPointF result;
    result.setX((int(pos.x()/gridInterval-0.5))*gridInterval);
    result.setY((int(pos.y()/gridInterval-0.5))*gridInterval);
    return result;
}

void NetworkEditor::initializeProcessorRepresentation(Processor* processor, QPointF pos) {
    // create and set processor widget(might be null if no widget exists)  
    ProcessorWidgetQt* processorWidgetQt = ProcessorWidgetFactoryQt::getRef().create(processor);
    processor->setProcessorWidget(processorWidgetQt);

    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem();
    processorGraphicsItem->setProcessor(processor);
    
    // TODO: if (!sceneRect().contains(pos)) CLAMP_TO_SCENE_RECT;
    if (gridSnapping_) pos = snapToGrid(pos);
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

    // FIXME: show property widgets, but then also deselect all other processors and select this one
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

    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->removeProcessorProperties(processor);

    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor) {
        processorNetworkEvaluator_->deRegisterCanvas(canvasProcessor->getCanvas());
    }     

    // remove GUI connections
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::instance()->connectionGraphicsItems_;
    for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
        if (connectionGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            connectionGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
            removeConnection(connectionGraphicsItems[i]);
        }
    }

    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems = NetworkEditor::instance()->linkGraphicsItems_;
    for (size_t i=0; i<linkGraphicsItems.size(); i++) {
        if (linkGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            linkGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
                removeLink(linkGraphicsItems[i]);
        }
    }

    processorNetwork_->removeProcessor(processor);
    processorGraphicsItem->setProcessor(0);
    processor->deinitialize();
    delete processor;

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

QGraphicsItem* NetworkEditor::getLinkGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    if (graphicsItems.size() > 0) {
        for (int i=0; i<graphicsItems.size(); i++) {
            LinkConnectionGraphicsItem* linkGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(graphicsItems[i]);
            if (linkGraphicsItem)
                return linkGraphicsItem;
        }
    }  
    return 0;
}

void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    startProcessor_ = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));

    //Link mode
    if (e->modifiers() == Qt::ControlModifier) {
       if (startProcessor_) {
           QRectF processorRect = startProcessor_->rect();
           processorRect = startProcessor_->mapToScene(processorRect).boundingRect();

           if (linkCurve_) delete linkCurve_;
           linkCurve_ = new LinkGraphicsItem(processorRect.center(), e->scenePos());
           linkCurve_->setZValue(2.0);
           addItem(linkCurve_);
           linkCurve_->show();          
       }
        e->accept();
       return;
    }

    if (startProcessor_) {
        startPort_ = startProcessor_->getSelectedPort(e->scenePos());
        if (startPort_ && startPort_->isOutport()) {
            QRectF portRect = startProcessor_->calculatePortRect(startPort_);
            portRect = startProcessor_->mapToScene(portRect).boundingRect();
            connectionCurve_ = new CurveGraphicsItem(portRect.center(), e->scenePos(), startPort_->getColorCode());
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
    } else {
        PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
        propertyListWidget_->removeAllProcessorProperties();
        QGraphicsScene::mousePressEvent(e);
    }
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {    

    //Link mode
    if (e->modifiers() == Qt::ControlModifier) {
        if (linkCurve_) {
            QPointF center = startProcessor_->getShortestBoundaryPointTo(e->scenePos());
            linkCurve_->setStartPoint(center) ;
            linkCurve_->setEndPoint(e->scenePos());
            linkCurve_->update();
            e->accept();
        } else
            QGraphicsScene::mouseMoveEvent(e);
        return;
    }

    if (connectionCurve_) {
        connectionCurve_->setEndPoint(e->scenePos());
        connectionCurve_->update();
        e->accept();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {

    //Link mode
    if (e->modifiers() == Qt::ControlModifier) {
        removeItem(linkCurve_);
        delete linkCurve_;
        linkCurve_ = 0;
        endProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (endProcessor_) {
            if (endProcessor_ != startProcessor_) {
                if (endProcessor_->getProcessor()->getProperties().size() && 
                    startProcessor_->getProcessor()->getProperties().size())
                    addLink(startProcessor_, endProcessor_);
                else
                    LogInfo("Processor must have properties to link.");
            }
        }
        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();
        return;
    }

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
    } else {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (processorGraphicsItem)
            if (gridSnapping_) processorGraphicsItem->setPos(snapToGrid(processorGraphicsItem->pos()));
        QGraphicsScene::mouseReleaseEvent(e);
    }
}

void NetworkEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkConnectionGraphicsItem* linkConnectionGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(getLinkGraphicsItemAt(e->scenePos()));

    if (linkConnectionGraphicsItem) {
        showLinkDialog(linkConnectionGraphicsItem);
        e->accept(); 
    }       
    else
        QGraphicsScene::mouseDoubleClickEvent(e);
}

void NetworkEditor::keyPressEvent(QKeyEvent* e) {
    // delete selected graphics items
    if (e->key() == Qt::Key_Delete) {
        // first delete connections & links
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem = dynamic_cast<ConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (connectionGraphicsItem)
                removeConnection(connectionGraphicsItem);
        }
        
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            LinkConnectionGraphicsItem* linkGraphicsItem = dynamic_cast<LinkConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (linkGraphicsItem)
                removeLink(linkGraphicsItem);
        }

        // second delete processors
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
            if (processorGraphicsItem)
                removeProcessor(processorGraphicsItem->getIdentifier());
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void NetworkEditor::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
    ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(getConnectionGraphicsItemAt(e->scenePos()));
    LinkConnectionGraphicsItem* linkConnectionGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(getLinkGraphicsItemAt(e->scenePos()));
    
    if (processorGraphicsItem) {
        QMenu menu;
        QAction* renameAction = menu.addAction("Rename");
        QAction* deleteAction = menu.addAction("Delete");

        QAction* showAction = 0;
        if (processorGraphicsItem->getProcessor()->hasProcessorWidget()) {
            showAction = menu.addAction("Show Widget");
            showAction->setCheckable(true);
            ProcessorWidgetQt* qtWidget = dynamic_cast<ProcessorWidgetQt*>(processorGraphicsItem->getProcessor()->getProcessorWidget());
            if(qtWidget && qtWidget->isHidden()) {
                showAction->setChecked(false);
            }
            else
                showAction->setChecked(true);
        }
        QAction* result = menu.exec(QCursor::pos());
        if (result == renameAction) {
            processorGraphicsItem->editProcessorName();
        } else if (result == deleteAction) {
            removeProcessor(processorGraphicsItem->getIdentifier());
        } else if (showAction && result == showAction) {
            if(showAction->isChecked()) {
                processorGraphicsItem->getProcessor()->getProcessorWidget()->show();
            } else
                processorGraphicsItem->getProcessor()->getProcessorWidget()->hide();
        }
    } else if (connectionGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == action)
            removeConnection(connectionGraphicsItem);
    } else if (linkConnectionGraphicsItem) {
        QMenu menu;
        QAction* linkAction = menu.addAction("Link Properties");
        QAction* deleteAction = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == deleteAction)
            removeLink(linkConnectionGraphicsItem);
        else if (result == linkAction) {
            Processor* inProcessor = linkConnectionGraphicsItem->getInProcessor()->getProcessor();
            Processor* outProcessor = linkConnectionGraphicsItem->getOutProcessor()->getProcessor();
            ProcessorLink* plink = processorNetwork_->getProcessorLink(inProcessor, outProcessor);
            if (plink) {
                plink->autoLinkPropertiesByType();
            }
        }

    } else
        QGraphicsScene::contextMenuEvent(e);
}

void NetworkEditor::showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem) {
    Processor* inProcessor = linkConnectionGraphicsItem->getInProcessor()->getProcessor();
    Processor* outProcessor = linkConnectionGraphicsItem->getOutProcessor()->getProcessor();

    LinkDialog* linkDialog = new LinkDialog(inProcessor, outProcessor, processorNetwork_, 0);
    linkDialog->exec();
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

void NetworkEditor::drawBackground(QPainter* painter, const QRectF & rect) {
    int gridInterval = 30;
    painter->setWorldMatrixEnabled(true);
    painter->fillRect(rect, Qt::darkGray);

    qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
    qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

    QVarLengthArray<QLineF, 100> linesX;
    for (qreal x = left; x < rect.right(); x += gridInterval )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;
    for (qreal y = top; y < rect.bottom(); y += gridInterval )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    IvwSerializer xmlSerializer(fileName);
    //xmlSerializer.serialize("NetworkEditor", *this) ;
    processorNetwork_->serialize(xmlSerializer);
    processorNetwork_->setModified(false);
    xmlSerializer.writeFile();    
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

    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (std::vector<ProcessorLink*>::iterator it = links.begin(); it!=links.end(); it++) {
        addLink(*it) ;
    }
    
    processorNetwork_->setModified(false);

    return true;
}

} // namespace