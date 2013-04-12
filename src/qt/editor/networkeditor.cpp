#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/util/inviwofactorybase.h>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>


#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>

namespace inviwo {

const std::string NetworkEditor::logSource_ = "NetworkEditor";
const int NetworkEditor::GRID_SPACING = 25;

NetworkEditor::NetworkEditor(QObject* parent) : QGraphicsScene(parent) {
    connectionCurve_ = 0;
    linkCurve_ = 0;
    startProcessor_ = 0;
    endProcessor_ = 0;
    inspectedPort_ = 0;
    gridSnapping_ = true;
    setSceneRect(-1000,-1000,1000,1000);

    processorNetwork_ = new ProcessorNetwork();
    InviwoApplication::getRef().setProcessorNetwork(processorNetwork_);
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);
}

void NetworkEditor::addProcessor(Processor* processor) {
    LogInfo("Adding processor.");
    // if identifier already exists in the network, generate a new identifier
    std::string identifier = processor->getClassName();
    if (processorNetwork_->getProcessorByName(identifier)) {
        unsigned int idNumber = 1;
        do {
            std::stringstream stringStream;
            stringStream << idNumber++;            
            identifier = processor->getClassName() + stringStream.str();
        } while (processorNetwork_->getProcessorByName(identifier));
    }
    processor->setIdentifier(identifier);
    processorNetwork_->addProcessor(processor);
}

void NetworkEditor::addProcessorGraphicsItem(Processor* processor, QPointF pos) {
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
}

void NetworkEditor::addProcessorWidget(Processor* processor) {
    // show processor widget
    // create and set processor widget(might be null if no widget exists)  
    ProcessorWidgetQt* processorWidgetQt = ProcessorWidgetFactoryQt::getRef().create(processor);
    processor->setProcessorWidget(processorWidgetQt);
    if (processor->hasProcessorWidget()) {
        processor->getProcessorWidget()->initialize();
        processor->getProcessorWidget()->show();
    }
}

void NetworkEditor::addPropertyWidgets(Processor* processor) {
    if (dynamic_cast<ProcessorMetaData*>(processor->getMetaData("ProcessorMetaData"))->isVisible()) {
        // FIXME: show property widgets, but then also deselect all other processors and select this one
        PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
        propertyListWidget_->showProcessorProperties(processor);
    }
}

void NetworkEditor::removePropertyWidgets(Processor* processor) {
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->removeProcessorProperties(processor);
}

void NetworkEditor::removeProcessor(Processor* processor) {
    LogInfo("Removing processor.");

    // deregister processors which act as initiation points for the network evaluation
    // TODO: generalize, should be done for all output processors
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor)
        processorNetworkEvaluator_->deregisterCanvas(canvasProcessor->getCanvas());

    processorNetwork_->removeProcessor(processor);
    // deinitialize also removes the processor's widget
    processor->deinitialize();
    delete processor;

    processorNetworkEvaluator_->evaluate();
}

void NetworkEditor::removeProcessorGraphicsItem(Processor* processor) {
    // obtain processor graphics item through processor
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(processor->getIdentifier());
    ivwAssert(processorGraphicsItem, "ProcessorGraphicsItem to be removed does not exist.")

    // remove connection graphics items
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = connectionGraphicsItems_;
    for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
        if (connectionGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            connectionGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
                removeConnectionGraphicsItem(connectionGraphicsItems[i]);
        }
    }

    // remove link graphics items
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems = linkGraphicsItems_;
    for (size_t i=0; i<linkGraphicsItems.size(); i++) {
        if (linkGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            linkGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
                removeLinkGraphicsItem(linkGraphicsItems[i]);
        }
    }

    // remove processor graphics item from editor
    processorGraphicsItem->hide();
    removeItem(processorGraphicsItem);
    processorGraphicsItems_.erase(std::remove(processorGraphicsItems_.begin(), processorGraphicsItems_.end(),
                                  processorGraphicsItem), processorGraphicsItems_.end());

    // delete processor graphics item
    processorGraphicsItem->setProcessor(0);
    delete processorGraphicsItem;
}

void NetworkEditor::addConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                              ProcessorGraphicsItem* inProcessor, Port* inport) {
    // generate GUI representation and add to editor
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(outProcessor, outport, inProcessor, inport);
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->show();
}

void NetworkEditor::removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem) {
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
}

void NetworkEditor::addLinkGraphicsItem(ProcessorGraphicsItem* outProcessor, ProcessorGraphicsItem* inProcessor) {
    LinkConnectionGraphicsItem* linkGraphicsItem = new LinkConnectionGraphicsItem(outProcessor, inProcessor);
    linkGraphicsItems_.push_back(linkGraphicsItem);
    addItem(linkGraphicsItem);
    linkGraphicsItem->show();
    showLinkDialog(linkGraphicsItem);
}

void NetworkEditor::removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem) {
    linkGraphicsItem->hide();
    removeItem(linkGraphicsItem);
    linkGraphicsItems_.erase(std::remove(linkGraphicsItems_.begin(), linkGraphicsItems_.end(), linkGraphicsItem),
                             linkGraphicsItems_.end());
}


void NetworkEditor::addInspectorNetwork(Port* port, ivec2 pos, std::string fileName) {
    IvwDeserializer xmlDeserializer(fileName);
    ProcessorNetwork* processorNetwork = new ProcessorNetwork();
    processorNetwork->deserialize(xmlDeserializer);

    std::vector<Processor*> processors = processorNetwork->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        Processor* processor = processors[i];
        processor->setIdentifier(port->getProcessor()->getIdentifier()+":"+port->getIdentifier()+":"+processor->getIdentifier());
        dynamic_cast<ProcessorMetaData*>(processor->getMetaData("ProcessorMetaData"))->setVisibile(false);
        processorNetwork_->addProcessor(processor);
        processor->initialize();
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
        if (canvasProcessor) {
            processor->setIdentifier("PortInspector "+port->getProcessor()->getIdentifier()+":"+port->getIdentifier());
            ProcessorWidgetQt* processorWidgetQt = ProcessorWidgetFactoryQt::getRef().create(processor);
            processorWidgetQt->setMaximumSize(256, 256);
            processorWidgetQt->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
            processor->setProcessorWidget(processorWidgetQt);
            processorWidgetQt->move(pos);
            processorWidgetQt->initialize();
            processorWidgetQt->show();
            processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());
        }
    }

    std::vector<PortConnection*> connections = processorNetwork->getPortConnections();
    for (size_t i=0; i<connections.size(); i++) {
        PortConnection* connection = connections[i];
        processorNetwork_->addConnection(connection->getOutport(), connection->getInport());
    }

    /*
    //TODO: establish links
    std::vector<ProcessorLink*> links = processorNetwork->getProcessorLinks();
    for (std::vector<ProcessorLink*>::iterator it = links.begin(); it!=links.end(); it++) {
        addLink(*it) ;
    }
    */

    for (size_t i=0; i<processors.size(); i++) {
        Processor* processor = processors[i];
        std::vector<Port*> inports = processor->getInports();
        for (size_t i=0; i<inports.size(); i++) {
            if (!inports[i]->isConnected()) {
                if (dynamic_cast<ImagePort*>(port)) {
                    if (dynamic_cast<ImagePort*>(inports[i]))
                        processorNetwork_->addConnection(port, inports[i]);
                } else if (dynamic_cast<VolumePort*>(port)) {
                    if (dynamic_cast<VolumePort*>(inports[i]))
                        processorNetwork_->addConnection(port, inports[i]);
                }
            }
        }
    }
}

void NetworkEditor::removeInspectorNetwork(Port* port) {
    std::string portPrefix = port->getProcessor()->getIdentifier()+":"+port->getIdentifier();
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0;i<processors.size();i++) {
        if (processors[i]->getIdentifier().find(portPrefix)!=std::string::npos)
            removeProcessor(processors[i]);
    }
}

void NetworkEditor::addPortInspector(Port* port, QPointF pos) {
    //TODO: allow to define inspectors in module
    ImagePort* imagePort = dynamic_cast<ImagePort*>(port);
    if (imagePort) {
        addInspectorNetwork(port, ivec2(pos.x(), pos.y()),
                            IVW_DIR+"data/workspaces/portinspectors/imageportinspector.inv");
        processorNetwork_->setModified(true);
        processorNetworkEvaluator_->evaluate();
        return;
    }
    VolumePort* volumePort = dynamic_cast<VolumePort*>(port);
    if (volumePort) {
        addInspectorNetwork(port, ivec2(pos.x(), pos.y()),
                            IVW_DIR+"data/workspaces/portinspectors/volumeportinspector.inv");
        processorNetwork_->setModified(true);
        processorNetworkEvaluator_->evaluate();
        return;
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
    for (int i=0; i<graphicsItems.size(); i++) {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItems[i]);
        if (processorGraphicsItem)
            return processorGraphicsItem;
    }
    return 0;
}

QGraphicsItem* NetworkEditor::getConnectionGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
        if (connectionGraphicsItem)
            return connectionGraphicsItem;
    }
    return 0;
}

QGraphicsItem* NetworkEditor::getLinkGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        LinkConnectionGraphicsItem* linkGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(graphicsItems[i]);
        if (linkGraphicsItem)
            return linkGraphicsItem;
    }
    return 0;
}

QPointF NetworkEditor::snapToGrid(QPointF pos) {
    QPointF result;
    result.setX((int(pos.x()/GRID_SPACING-0.5))*GRID_SPACING);
    result.setY((int(pos.y()/GRID_SPACING-0.5))*GRID_SPACING);
    return result;
}

void NetworkEditor::drawBackground(QPainter* painter, const QRectF & rect) {
    painter->setWorldMatrixEnabled(true);
    painter->fillRect(rect, Qt::darkGray);

    qreal left = int(rect.left()) - (int(rect.left()) % GRID_SPACING );
    qreal top = int(rect.top()) - (int(rect.top()) % GRID_SPACING );

    QVarLengthArray<QLineF, 100> linesX;
    for (qreal x = left; x < rect.right(); x += GRID_SPACING )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;
    for (qreal y = top; y < rect.bottom(); y += GRID_SPACING )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}

void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        startProcessor_ = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (startProcessor_) {

            if (e->modifiers()==Qt::NoModifier) {
                // edit connections
                startPort_ = startProcessor_->getSelectedPort(e->scenePos());

                if (startPort_ && startPort_->isOutport()) {
                    // click on outport: start drawing a connection
                    QRectF portRect = startProcessor_->calculatePortRect(startPort_);
                    portRect = startProcessor_->mapToScene(portRect).boundingRect();
                    connectionCurve_ = new CurveGraphicsItem(portRect.center(), e->scenePos(), startPort_->getColorCode());
                    addItem(connectionCurve_);
                    connectionCurve_->show();
                    e->accept();

                } else if (startPort_ && startPort_->isInport()) {
                    // click on inport: disconnect if connected
                    if (startPort_->isConnected()) {
                        // first remove existing connection
                        std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
                        for (size_t i=0; i<portConnections.size(); i++) {
                            const Port* curInport = portConnections[i]->getInport();
                            if (curInport == startPort_) {
                                startPort_ = portConnections[i]->getOutport();
                                continue;
                            }
                        }
                        ConnectionGraphicsItem* connectionGraphicsItem =
                            qgraphicsitem_cast<ConnectionGraphicsItem*>(getConnectionGraphicsItemAt(e->scenePos()));
                        processorNetwork_->removeConnection(connectionGraphicsItem->getInport(),
                            connectionGraphicsItem->getOutport());
                        removeConnectionGraphicsItem(connectionGraphicsItem);

                        /// initialize parameters for new connection
                        QPointF startPoint = connectionGraphicsItem->getStartPoint();
                        startProcessor_ = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(startPoint));

                        // generate new curve
                        connectionCurve_ = new CurveGraphicsItem(startPoint, e->scenePos(), startPort_->getColorCode());
                        addItem(connectionCurve_);
                        connectionCurve_->show();
                        e->accept();
                    }

                } else {
                    // click on processor but not on port: show property widgets
                    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
                    propertyListWidget_->showProcessorProperties(startProcessor_->getProcessor());
                    QGraphicsScene::mousePressEvent(e);
                }

            } else if (e->modifiers() == Qt::ControlModifier) {
                // ctrl modifier pressed: edit link
                QRectF processorRect = startProcessor_->mapToScene(processorRect).boundingRect();
                linkCurve_ = new LinkGraphicsItem(processorRect.center(), e->scenePos());
                addItem(linkCurve_);
                linkCurve_->show();
                e->accept();
            }
        } else {
            // no processor selected
            PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
            propertyListWidget_->removeAllProcessorProperties();
            QGraphicsScene::mousePressEvent(e);
        }
    }
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // connection drag mode
        connectionCurve_->setEndPoint(e->scenePos());
        connectionCurve_->update();
        e->accept();

    } else if (linkCurve_) {
        // link drag mode
        QPointF center = startProcessor_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        linkCurve_->update();
        e->accept();

    } else {
        /*
        if (e->button()==Qt::NoButton) {
            // port inspector hover effect
            ProcessorGraphicsItem* processor = qgraphicsitem_cast<ProcessorGraphicsItem*>(
                                                    getProcessorGraphicsItemAt(e->scenePos()));
            Port* port = 0;
            if (processor) port = processor->getSelectedPort(e->scenePos());
            if (processor && port) {
                // show port inspector
                if (inspectedPort_ != port && port->isOutport()) {
                    inspectedPort_ = port;
                    addPortInspector(port, e->screenPos());
                }
            } else {
                // remove port inspector
                if (inspectedPort_) {
                    removeInspectorNetwork(inspectedPort_);
                    inspectedPort_ = 0;
                }
            }
        }
        */
    }
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // connection drag mode
        removeItem(connectionCurve_);
        delete connectionCurve_;
        connectionCurve_ = 0;
        endProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (endProcessor_) {
            endPort_ = endProcessor_->getSelectedPort(e->scenePos());
            if (endPort_ && !endPort_->isOutport()) {
                processorNetwork_->addConnection(startPort_, endPort_);
                addConnectionGraphicsItem(startProcessor_, startPort_, endProcessor_, endPort_);
                processorNetworkEvaluator_->evaluate();
            }
        }
        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();
    
    } else if (linkCurve_) {
        // link drag mode
        removeItem(linkCurve_);
        delete linkCurve_;
        linkCurve_ = 0;
        endProcessor_ = dynamic_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (endProcessor_) {
            // FIXME: Links within one processor should/could also be possible
            if (endProcessor_ != startProcessor_) {
                if (startProcessor_->getProcessor()->getProperties().size() && 
                    endProcessor_->getProcessor()->getProperties().size()) {
                    processorNetwork_->addLink(startProcessor_->getProcessor(), endProcessor_->getProcessor());
                    addLinkGraphicsItem(startProcessor_, endProcessor_);
                    processorNetworkEvaluator_->evaluate();
                } else
                    LogInfo("Both processors must have properties to link.");
            }
        }
        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();

    } else {
        // move processor
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
        if (processorGraphicsItem)
            if (gridSnapping_) processorGraphicsItem->setPos(snapToGrid(processorGraphicsItem->pos()));
        QGraphicsScene::mouseReleaseEvent(e);
    }
}

void NetworkEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkConnectionGraphicsItem* linkConnectionGraphicsItem =
        qgraphicsitem_cast<LinkConnectionGraphicsItem*>(getLinkGraphicsItemAt(e->scenePos()));
    if (linkConnectionGraphicsItem) {
        // link edit mode
        showLinkDialog(linkConnectionGraphicsItem);
        e->accept(); 
        processorNetworkEvaluator_->evaluate();

    } else {
        QGraphicsScene::mouseDoubleClickEvent(e);
    }
}

void NetworkEditor::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Delete) {
        // delete selected graphics items

        // check if selected is connection and delete it
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem = dynamic_cast<ConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (connectionGraphicsItem) {
                removeConnectionGraphicsItem(connectionGraphicsItem);
                processorNetwork_->removeConnection(connectionGraphicsItem->getInport(),
                                                    connectionGraphicsItem->getOutport());
                delete connectionGraphicsItem;
                processorNetworkEvaluator_->evaluate();
            }
        }
        
        // check if selected is link and delete it
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            LinkConnectionGraphicsItem* linkGraphicsItem = dynamic_cast<LinkConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (linkGraphicsItem) {
                removeLinkGraphicsItem(linkGraphicsItem);
                processorNetwork_->removeLink(linkGraphicsItem->getOutProcessor()->getProcessor(),
                                              linkGraphicsItem->getInProcessor()->getProcessor());
                delete linkGraphicsItem;
                processorNetworkEvaluator_->evaluate();
            }
        }

        // check if selected is processor and delete it
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
            if (processorGraphicsItem) {
                Processor* processor = processorGraphicsItem->getProcessor();
                removeProcessorGraphicsItem(processor);
                removeProcessor(processor);
                processorNetworkEvaluator_->evaluate();
            }
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void NetworkEditor::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(getProcessorGraphicsItemAt(e->scenePos()));
    ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(getConnectionGraphicsItemAt(e->scenePos()));
    LinkConnectionGraphicsItem* linkConnectionGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(getLinkGraphicsItemAt(e->scenePos()));
    
    if (processorGraphicsItem) {
        Port* selectedPort = processorGraphicsItem->getSelectedPort(e->scenePos());
        if (selectedPort && selectedPort->isOutport()) {
            // Port context menu
            QMenu menu;
            QAction* inspectorAction = 0;
            inspectorAction = menu.addAction("Inspect port");
            inspectorAction->setCheckable(true);
            bool inspectorShown = false;
            if (selectedPort->isConnected()) {
                std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
                for (size_t i=0;i<portConnections.size();i++) {
                    if (portConnections[i]->getOutport()==selectedPort) {
                        std::string portPrefix = selectedPort->getProcessor()->getIdentifier()+":"+selectedPort->getIdentifier();
                        std::string connectedProcessorIdentifier = portConnections[i]->getInport()->getProcessor()->getIdentifier();
                        if (connectedProcessorIdentifier.find(portPrefix)!=std::string::npos)
                            inspectorShown = true;
                    }
                }
            }
            if (inspectorShown) inspectorAction->setChecked(true);
            else inspectorAction->setChecked(false);

            QAction* result = menu.exec(QCursor::pos());
            if (result == inspectorAction)
                if (inspectorAction->isChecked())
                    addPortInspector(selectedPort, e->screenPos());
                else
                    removeInspectorNetwork(selectedPort);
        } else {
            // Processor context menu
            QMenu menu;
            QAction* renameAction = menu.addAction(tr("Rename"));
            QAction* deleteAction = menu.addAction(tr("Delete"));

            QAction* showAction = 0;
            if (processorGraphicsItem->getProcessor()->hasProcessorWidget()) {
                showAction = menu.addAction(tr("Show processor widget"));
                showAction->setCheckable(true);
                ProcessorWidgetQt* qtWidget = dynamic_cast<ProcessorWidgetQt*>(processorGraphicsItem->getProcessor()->getProcessorWidget());
                if (qtWidget && qtWidget->isHidden()) showAction->setChecked(false);
                else showAction->setChecked(true);
            }
            QAction* result = menu.exec(QCursor::pos());
            if (result == renameAction) {
                processorGraphicsItem->editProcessorName();
            } else if (result == deleteAction) {
                Processor* processor = processorGraphicsItem->getProcessor();
                removeProcessorGraphicsItem(processor);
                removeProcessor(processor);
                processorNetworkEvaluator_->evaluate();
            } else if (showAction && result == showAction) {
                if(showAction->isChecked())
                    processorGraphicsItem->getProcessor()->getProcessorWidget()->show();
                else
                    processorGraphicsItem->getProcessor()->getProcessorWidget()->hide();
            }
        }
    } else if (connectionGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction(tr("Delete"));
        QAction* result = menu.exec(QCursor::pos());
        if (result == action)
            processorNetwork_->removeConnection(connectionGraphicsItem->getInport(),
                                                connectionGraphicsItem->getOutport());
    } else if (linkConnectionGraphicsItem) {
        QMenu menu;
        QAction* linkAction = menu.addAction(tr("Link properties"));
        QAction* deleteAction = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == deleteAction)
            processorNetwork_->removeLink(linkConnectionGraphicsItem->getOutProcessor()->getProcessor(),
                                          linkConnectionGraphicsItem->getInProcessor()->getProcessor());
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
            // create processor and add it to data flow network    
            Processor* processor = dynamic_cast<Processor*>(ProcessorFactory::getRef().create(className.toStdString()));
            addProcessor(processor);
            addProcessorGraphicsItem(processor, e->scenePos());
            processor->initialize();

            // add relevant widgets
            addProcessorWidget(processor);
            addPropertyWidgets(processor);

            // TODO: Generalize by registering output processors (can also be e.g. VolumeSave)
            CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
            if (canvasProcessor)
                processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());

            e->setAccepted(true);
            e->acceptProposedAction();
        }
    }
}

void NetworkEditor::clearNetwork() {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if (dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"))->isVisible())
            removeProcessorGraphicsItem(processors[i]);
        removeProcessor(processors[i]);
    }
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    IvwSerializer xmlSerializer(fileName);
    processorNetwork_->serialize(xmlSerializer);
    processorNetwork_->setModified(false);
    xmlSerializer.writeFile();
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {
    clearNetwork();
    IvwDeserializer xmlDeserializer(fileName);
    processorNetwork_->deserialize(xmlDeserializer);

    // add processor graphics items, register output processors, and show widgets
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (std::vector<Processor*>::iterator it = processors.begin(); it!=processors.end(); it++) {
        Processor* processor = *it;
        ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processor->getMetaData("ProcessorMetaData"));
        if (meta->isVisible()) {
            QPointF pos = QPointF(meta->getPosition().x, meta->getPosition().y);
            addProcessorGraphicsItem(processor, pos);
        }
        processor->initialize();

        addProcessorWidget(processor);
        if (meta->isVisible()) addPropertyWidgets(processor);

        // TODO: Generalize by registering output processors (can also be e.g. VolumeSave)
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
        if (canvasProcessor)
            processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(),
            canvasProcessor->getIdentifier());
    }

    // add connection graphics items
    std::vector<PortConnection*> connections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<connections.size(); i++) {
        Processor* outProcessor = connections[i]->getOutport()->getProcessor();
        Processor* inProcessor = connections[i]->getInport()->getProcessor();
        if (dynamic_cast<ProcessorMetaData*>(outProcessor->getMetaData("ProcessorMetaData"))->isVisible() &&
            dynamic_cast<ProcessorMetaData*>(inProcessor->getMetaData("ProcessorMetaData"))->isVisible()) {
                ProcessorGraphicsItem* outProcessorItem = getProcessorGraphicsItem(outProcessor->getIdentifier());
                ProcessorGraphicsItem* inProcessorItem = getProcessorGraphicsItem(inProcessor->getIdentifier());
                addConnectionGraphicsItem(outProcessorItem, connections[i]->getOutport(),
                                          inProcessorItem, connections[i]->getInport());
        }
    }

    // add link graphics items
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        Processor* outProcessor = links[i]->getOutProcessor();
        Processor* inProcessor = links[i]->getInProcessor();
        ProcessorGraphicsItem* outProcessorItem = getProcessorGraphicsItem(outProcessor->getIdentifier());
        ProcessorGraphicsItem* inProcessorItem = getProcessorGraphicsItem(inProcessor->getIdentifier());
        if (dynamic_cast<ProcessorMetaData*>(outProcessor->getMetaData("ProcessorMetaData"))->isVisible() &&
            dynamic_cast<ProcessorMetaData*>(inProcessor->getMetaData("ProcessorMetaData"))->isVisible()) {
            addLinkGraphicsItem(outProcessorItem, inProcessorItem);
        }
    }
    
    processorNetwork_->setModified(false);
    return true;
}

} // namespace