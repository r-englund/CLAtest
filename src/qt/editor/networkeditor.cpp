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



/////////////////////////////////////////////
//   PUBLIC METHODS FOR CHANGING NETWORK   //
/////////////////////////////////////////////
void NetworkEditor::addProcessor(Processor* processor, QPointF pos) {
    // add the processor to the network
    processor->setIdentifier(obtainUniqueProcessorID(processor));
    processor->initialize();
    processorNetwork_->addProcessor(processor);
    // add processor representations
    addProcessorRepresentations(processor, pos);
}

void NetworkEditor::removeProcessor(Processor* processor) {
    // remove processor representations
    removeProcessorRepresentations(processor);
    // remove the processor from the network
    processorNetwork_->removeProcessor(processor);
    processor->deinitialize();
    delete processor;
}


void NetworkEditor::addConnection(Port* port1, Port* port2) {
    processorNetwork_->addConnection(port1, port2);
    addConnectionGraphicsItem(port1, port2);
}

void NetworkEditor::removeConnection(Port* port1, Port* port2) {
    removeConnectionGraphicsItem(getConnectionGraphicsItem(port1, port2));
    processorNetwork_->removeConnection(port1, port2);
}


void NetworkEditor::addLink(Processor* processor1, Processor* processor2) {
    processorNetwork_->addLink(processor1, processor2);
    addLinkGraphicsItem(processor1, processor2);
}

void NetworkEditor::removeLink(Processor* processor1, Processor* processor2) {
    removeLinkGraphicsItem(getLinkGraphicsItem(processor1, processor2));
    processorNetwork_->removeLink(processor1, processor2);
}



////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING PROCESSORS   //
////////////////////////////////////////////////////////
void NetworkEditor::addProcessorRepresentations(Processor* processor, QPointF pos) {
    // generate GUI representations (graphics item, property widget, processor widget)
    ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processor->getMetaData("ProcessorMetaData"));
    if (meta->isVisible()) {
        addProcessorGraphicsItem(processor, pos);
        addPropertyWidgets(processor);
    }
    addProcessorWidget(processor);

    // TODO: Generalize by registering output processors (can also be e.g. VolumeSave)
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor)
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());
}

void NetworkEditor::removeProcessorRepresentations(Processor* processor) {
    // deregister processors which act as initiation points for the network evaluation
    // TODO: generalize, should be done for all output processors
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor)
        processorNetworkEvaluator_->deregisterCanvas(canvasProcessor->getCanvas());

    if (dynamic_cast<ProcessorMetaData*>(processor->getMetaData("ProcessorMetaData"))->isVisible())
        removeProcessorGraphicsItem(processor);
    removePropertyWidgets(processor);
    // processor widget already removed when processor is destroyed
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

void NetworkEditor::addPropertyWidgets(Processor* processor) {
    // FIXME: show property widgets, but then also deselect all other processors and select this one
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->showProcessorProperties(processor);
}

void NetworkEditor::removePropertyWidgets(Processor* processor) {
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->removeProcessorProperties(processor);
}

// remove processor widget unnecessary as processor widget is removed when processor is destroyed
void NetworkEditor::addProcessorWidget(Processor* processor) {
    ProcessorWidgetQt* processorWidgetQt = ProcessorWidgetFactoryQt::getRef().create(processor);
    if (processorWidgetQt) {
        processor->setProcessorWidget(processorWidgetQt);
        processor->getProcessorWidget()->initialize();
        processor->getProcessorWidget()->show();
    }
}



/////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING CONNECTIONS   //
/////////////////////////////////////////////////////////
void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    Port* port1 = connectionGraphicsItem->getOutport();
    Port* port2 = connectionGraphicsItem->getInport();
    removeConnectionGraphicsItem(connectionGraphicsItem);
    processorNetwork_->removeConnection(port1, port2);
}

void NetworkEditor::addConnectionGraphicsItem(Port* port1, Port* port2) {
    ProcessorGraphicsItem* processor1 = getProcessorGraphicsItem(port1->getProcessor()->getIdentifier());
    ProcessorGraphicsItem* processor2 = getProcessorGraphicsItem(port2->getProcessor()->getIdentifier());
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(processor1, port1,
                                                                                processor2, port2);
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->show();
}

void NetworkEditor::removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem) {
    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
        connectionGraphicsItem), connectionGraphicsItems_.end());
    delete connectionGraphicsItem;
}



///////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING LINKS   //
///////////////////////////////////////////////////
void NetworkEditor::removeLink(LinkConnectionGraphicsItem* linkGraphicsItem) {
    Processor* processor1 = linkGraphicsItem->getOutProcessor()->getProcessor();
    Processor* processor2 = linkGraphicsItem->getInProcessor()->getProcessor();
    removeLinkGraphicsItem(linkGraphicsItem);
    processorNetwork_->removeLink(processor1, processor2);
}

void NetworkEditor::addLinkGraphicsItem(Processor* processor1, Processor* processor2) {
    ProcessorGraphicsItem* processorGraphicsItem1 = getProcessorGraphicsItem(processor1->getIdentifier());
    ProcessorGraphicsItem* processorGraphicsItem2 = getProcessorGraphicsItem(processor2->getIdentifier());
    LinkConnectionGraphicsItem* linkGraphicsItem = new LinkConnectionGraphicsItem(processorGraphicsItem1,
                                                                                  processorGraphicsItem2);
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
    delete linkGraphicsItem;
}

void NetworkEditor::showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem) {
    Processor* inProcessor = linkConnectionGraphicsItem->getInProcessor()->getProcessor();
    Processor* outProcessor = linkConnectionGraphicsItem->getOutProcessor()->getProcessor();

    LinkDialog* linkDialog = new LinkDialog(inProcessor, outProcessor, processorNetwork_, 0);
    linkDialog->exec();
}



//////////////////////////////////////
//   PORT INSPECTOR FUNCTIONALITY   //
//////////////////////////////////////
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
        //processor->initialize();
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
        if (processors[i]->getIdentifier().find(portPrefix)!=std::string::npos) {
            removeProcessorRepresentations(processors[i]);
            removeProcessor(processors[i]);
        }
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



////////////////////////////////////////////
//   OBTAIN GRAPHICS ITEMS FROM NETWORK   //
////////////////////////////////////////////
ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItem(std::string identifier) const {
    for (size_t i=0; i<processorGraphicsItems_.size(); i++)
        if (processorGraphicsItems_[i]->getIdentifier() == identifier)
            return processorGraphicsItems_[i];
    return 0;
}

ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItem(Port* port1, Port* port2) const {
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        if (connectionGraphicsItems_[i]->getOutport() == port1 &&
            connectionGraphicsItems_[i]->getOutport() == port2)
            return connectionGraphicsItems_[i];
    return 0;
}

LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItem(Processor* processor1, Processor* processor2) const {
    for (size_t i=0; i<linkGraphicsItems_.size(); i++)
        if (linkGraphicsItems_[i]->getOutProcessor()->getProcessor() == processor1 &&
            linkGraphicsItems_[i]->getInProcessor()->getProcessor() == processor2)
            return linkGraphicsItems_[i];
    return 0;
}

ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        ProcessorGraphicsItem* processorGraphicsItem = qgraphicsitem_cast<ProcessorGraphicsItem*>(graphicsItems[i]);
        if (processorGraphicsItem)
            return processorGraphicsItem;
    }
    return 0;
}

ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        ConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
        if (connectionGraphicsItem)
            return connectionGraphicsItem;
    }
    return 0;
}

LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItemAt(const QPointF pos) const {
    QList<QGraphicsItem*> graphicsItems = items(pos);
    for (int i=0; i<graphicsItems.size(); i++) {
        LinkConnectionGraphicsItem* linkGraphicsItem = qgraphicsitem_cast<LinkConnectionGraphicsItem*>(graphicsItems[i]);
        if (linkGraphicsItem)
            return linkGraphicsItem;
    }
    return 0;
}



////////////////////////////////
//   EVENT HANDLING METHODS   //
////////////////////////////////
void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        startProcessor_ = getProcessorGraphicsItemAt(e->scenePos());
        if (startProcessor_) {

            if (e->modifiers()==Qt::NoModifier) {
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
                    /*
                    // FIXME: delete operation in release event handling results in a crash when
                    //        disconnecting two ports through drag and drop
                    if (startPort_->isConnected()) {
                        // first remove existing connection, and remember start port
                        std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
                        for (size_t i=0; i<portConnections.size(); i++) {
                            Port* curInport = portConnections[i]->getInport();
                            if (curInport == startPort_)
                                startPort_ = portConnections[i]->getOutport();
                        }

                        /// initialize parameters for new connection
                        ConnectionGraphicsItem* connectionGraphicsItem = getConnectionGraphicsItemAt(e->scenePos());
                        QPointF startPoint = connectionGraphicsItem->getStartPoint();
                        removeConnection(connectionGraphicsItem);
                        startProcessor_ = getProcessorGraphicsItemAt(startPoint);

                        // generate new curve
                        connectionCurve_ = new CurveGraphicsItem(startPoint, e->scenePos(), startPort_->getColorCode());
                        addItem(connectionCurve_);
                        connectionCurve_->show();
                        */
                        e->accept();
                    //}

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
        // hover effect for inspector networks
        /*
        if (e->button()==Qt::NoButton) {
            // port inspector hover effect
            ProcessorGraphicsItem* processor = getProcessorGraphicsItemAt(e->scenePos());
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
        } else
        */
        QGraphicsScene::mouseMoveEvent(e);
    }
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // connection drag mode
        removeItem(connectionCurve_);
        delete connectionCurve_;
        connectionCurve_ = 0;

        endProcessor_ = getProcessorGraphicsItemAt(e->scenePos());
        if (endProcessor_) {
            endPort_ = endProcessor_->getSelectedPort(e->scenePos());
            if (endPort_ && !endPort_->isOutport() && !endPort_->isConnected())
                addConnection(startPort_, endPort_);
        }

        startProcessor_ = 0;
        endProcessor_ = 0;
        e->accept();
    
    } else if (linkCurve_) {
        // link drag mode
        removeItem(linkCurve_);
        delete linkCurve_;
        linkCurve_ = 0;
        /*
        endProcessor_ = getProcessorGraphicsItemAt(e->scenePos());
        if (endProcessor_)
            addLink(startProcessor_->getProcessor(), endProcessor_->getProcessor());
            */
        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();

    } else {
        // move processor
        ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItemAt(e->scenePos());
        if (processorGraphicsItem)
            if (gridSnapping_) processorGraphicsItem->setPos(snapToGrid(processorGraphicsItem->pos()));
        QGraphicsScene::mouseReleaseEvent(e);
    }
}

void NetworkEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkConnectionGraphicsItem* linkGraphicsItem = getLinkGraphicsItemAt(e->scenePos());
    if (linkGraphicsItem) {
        // link edit mode
        showLinkDialog(linkGraphicsItem);
        e->accept();

    } else
        QGraphicsScene::mouseDoubleClickEvent(e);
}

void NetworkEditor::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Delete) {
        // delete selected graphics items

        // check if selected is connection and delete it
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem = dynamic_cast<ConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (connectionGraphicsItem)
                removeConnection(connectionGraphicsItem);
        }
        
        // check if selected is link and delete it
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            LinkConnectionGraphicsItem* linkGraphicsItem = dynamic_cast<LinkConnectionGraphicsItem*>(selectedGraphicsItems[i]);
            if (linkGraphicsItem)
                removeLink(linkGraphicsItem);
        }

        // check if selected is processor and delete it
        selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
            if (processorGraphicsItem) {
                Processor* processor = processorGraphicsItem->getProcessor();
                removeProcessor(processor);
            }
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void NetworkEditor::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItemAt(e->scenePos());
    ConnectionGraphicsItem* connectionGraphicsItem = getConnectionGraphicsItemAt(e->scenePos());
    LinkConnectionGraphicsItem* linkGraphicsItem = getLinkGraphicsItemAt(e->scenePos());
    
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
                // check if outport is already connected to an inspector network
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
                ProcessorWidgetQt* processorWidget = dynamic_cast<ProcessorWidgetQt*>(processorGraphicsItem->getProcessor()->getProcessorWidget());
                if (processorWidget && processorWidget->isHidden()) showAction->setChecked(false);
                else showAction->setChecked(true);
            }
            QAction* result = menu.exec(QCursor::pos());
            if (result == renameAction) {
                processorGraphicsItem->editProcessorName();
            } else if (result == deleteAction) {
                Processor* processor = processorGraphicsItem->getProcessor();
                removeProcessor(processor);
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
            removeConnection(connectionGraphicsItem);

    } else if (linkGraphicsItem) {
        QMenu menu;
        QAction* linkAction = menu.addAction(tr("Link properties"));
        QAction* deleteAction = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == deleteAction)
            removeLink(linkGraphicsItem);

        else if (result == linkAction) {
            Processor* inProcessor = linkGraphicsItem->getInProcessor()->getProcessor();
            Processor* outProcessor = linkGraphicsItem->getOutProcessor()->getProcessor();
            ProcessorLink* processorLink = processorNetwork_->getProcessorLink(inProcessor, outProcessor);
            if (processorLink)
                processorLink->autoLinkPropertiesByType();
        }

    } else
        QGraphicsScene::contextMenuEvent(e);
}



/////////////////////////////////////////
//   PROCESSOR DRAG AND DROP METHODS   //
/////////////////////////////////////////
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
            // create processor, add it to processor network, and generate it's widgets
            Processor* processor = dynamic_cast<Processor*>(ProcessorFactory::getRef().create(className.toStdString()));
            addProcessor(processor, e->scenePos());
            e->setAccepted(true);
            e->acceptProposedAction();
        }
    }
}



///////////////////////////////
//   SERIALIZATION METHODS   //
///////////////////////////////
void NetworkEditor::clearNetwork() {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        removeProcessor(processors[i]);
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    IvwSerializer xmlSerializer(fileName);
    processorNetwork_->serialize(xmlSerializer);
    processorNetwork_->setModified(false);
    xmlSerializer.writeFile();
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {
    // first we clean the current network
    clearNetwork();

    // then we deserialize into an intermediate processor network
    IvwDeserializer xmlDeserializer(fileName);
    processorNetwork_->deserialize(xmlDeserializer);

    // add processors
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        processors[i]->initialize();
        ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));
        addProcessorRepresentations(processors[i], QPointF(meta->getPosition().x, meta->getPosition().y));
    }

    // add connections
    std::vector<PortConnection*> connections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<connections.size(); i++)
        addConnectionGraphicsItem(connections[i]->getOutport(), connections[i]->getInport());
        //addConnectionGraphicsItem(connections[i]);
        //ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(processor1, port1,
                                                                            //processor2, port2);


    // add link graphics items
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++)
        addLinkGraphicsItem(links[i]->getOutProcessor(), links[i]->getInProcessor());
        /*
        Processor* outProcessor = links[i]->getOutProcessor();
        Processor* inProcessor = links[i]->getInProcessor();
        ProcessorGraphicsItem* outProcessorItem = getProcessorGraphicsItem(outProcessor->getIdentifier());
        ProcessorGraphicsItem* inProcessorItem = getProcessorGraphicsItem(inProcessor->getIdentifier());
        if (dynamic_cast<ProcessorMetaData*>(outProcessor->getMetaData("ProcessorMetaData"))->isVisible() &&
            dynamic_cast<ProcessorMetaData*>(inProcessor->getMetaData("ProcessorMetaData"))->isVisible()) {
            addLinkGraphicsItem(outProcessorItem, inProcessorItem);
        }
        */
    
    // flag the network's modified flag to initiate evaluation
    processorNetwork_->setModified(true);

    return true;
}



////////////////////////
//   HELPER METHODS   //
////////////////////////
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

std::string NetworkEditor::obtainUniqueProcessorID(Processor* processor) const {
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
    return identifier;
}

} // namespace