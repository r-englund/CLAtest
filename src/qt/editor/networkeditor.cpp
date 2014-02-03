/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/


#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>
#include <QGraphicsItem>

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/multidatainport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/util/settings/linksettings.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/ports/portinspectorfactory.h>
#include <inviwo/core/resources/resourcemanager.h>


namespace inviwo {

const int NetworkEditor::GRID_SPACING = 25;

NetworkEditor::NetworkEditor() :
    QGraphicsScene()
    , oldConnectionTarget_(NULL)
    , oldProcessorTarget_(NULL)
    , connectionCurve_(0)
    , linkCurve_(0)
    , startProcessor_(0)
    , endProcessor_(0)
    , inspection_()
    , gridSnapping_(true)
    , filename_("") {
    setSceneRect(-1000,-1000,1000,1000);
    workerThreadReset();
    processorNetwork_ = new ProcessorNetwork();
    InviwoApplication::getRef().setProcessorNetwork(processorNetwork_);
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);
    hoverTimer_.setSingleShot(true);
    connect(&hoverTimer_, SIGNAL(timeout()), this, SLOT(managePortInspectors()));
}

#define DELETE_VECTOR_ENTRIES(vec) while(!vec.empty()){delete vec.back();vec.pop_back();}

NetworkEditor::~NetworkEditor() {
    workerThreadQuit();
    processorNetwork_->lock();
    processorNetwork_->setBroadcastModification(false);

    removePortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_);

    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    while (!processors.empty()) {
        removeProcessor(processors.back());
        processors.pop_back();
    }

    delete processorNetwork_;
    delete processorNetworkEvaluator_;
    ivwAssert(processorGraphicsItems_.size()==0,
              "ProcessorGraphicsItems not properly removed");
    ivwAssert(connectionGraphicsItems_.size() == 0,
              "ConnectionGraphicsItems not properly removed");
    ivwAssert(linkGraphicsItems_.size() == 0,
              "LinkGraphicsItems not properly removed");
}

/////////////////////////////////////////////
//   PUBLIC METHODS FOR CHANGING NETWORK   //
/////////////////////////////////////////////
void NetworkEditor::addProcessor(Processor* processor, QPointF pos, bool showProcessor, bool selectProcessor, bool showPropertyWidgets,
                                 bool showProcessorWidget) {
    // add the processor to the network
    processor->setIdentifier(obtainUniqueProcessorID(processor->getClassName()));
    processor->initialize();
    processorNetwork_->addProcessor(processor);
    // add processor representations
    addProcessorRepresentations(processor, pos, showProcessor, showPropertyWidgets, selectProcessor);
    autoLinkOnAddedProcessor(processor);
}

void NetworkEditor::removeProcessor(Processor* processor) {
    removePortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_);
    // remove processor representations
    removeProcessorRepresentations(processor);
    // enable default rendering context before deinitializing processor
    processorNetworkEvaluator_->activateDefaultRenderContext();
    // remove the processor from the network
    processorNetwork_->removeAndDeleteProcessor(processor);
}


void NetworkEditor::addConnection(Outport* outport, Inport* inport) {
    processorNetwork_->addConnection(outport, inport);
    addConnectionGraphicsItem(outport, inport);
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(inport->getProcessor());
    ImageInport* imageInport = dynamic_cast<ImageInport*>(inport);
    ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(outport);

    if (canvasProcessor && imageInport) {
        ResizeEvent resizeEvent(canvasProcessor->getCanvas()->getDimension());
        imageInport->changeDataDimensions(&resizeEvent);
    }
    else if (!canvasProcessor && imageInport) {
        ResizeEvent resizeEvent(imageOutport->getDimension());
        imageInport->changeDataDimensions(&resizeEvent);
    }
}

void NetworkEditor::removeConnection(Outport* outport, Inport* inport) {
    removeConnectionGraphicsItem(getConnectionGraphicsItem(outport, inport));
    processorNetwork_->removeConnection(outport, inport);
}


void NetworkEditor::addLink(Processor* processor1, Processor* processor2) {
    if (!processorNetwork_->getLink(processor1, processor2)) {
        processorNetwork_->addLink(processor1, processor2);
        addLinkGraphicsItem(processor1, processor2);
    }
}

void NetworkEditor::removeLink(Processor* processor1, Processor* processor2) {
    removeLinkGraphicsItem(getLinkGraphicsItem(processor1, processor2));
    processorNetwork_->removeLink(processor1, processor2);
}

void NetworkEditor::autoLinkOnAddedProcessor(Processor* addedProcessor) {
    std::vector<Processor*> existingProcessors = processorNetwork_->getProcessors();
    std::vector<Property*> dstProperties = addedProcessor->getProperties();
    LinkSettings* linkSettings = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>();

    for (size_t i=0; i<dstProperties.size(); i++) {
        Property* dstProperty = dstProperties[i];

        for (size_t j=0; j<existingProcessors.size(); j++) {
            if (existingProcessors[j]!=addedProcessor) {
                std::vector<Property*> srcProperties = existingProcessors[j]->getProperties();
                Property* srcProperty = 0;

                for (size_t k=0; k<srcProperties.size(); k++) {
                    if (AutoLinker::canLink(srcProperties[k], dstProperty, DefaultLinkingCondition) &&
                        linkSettings->isLinkable(srcProperties[k]) &&
                        linkSettings->isLinkable(dstProperty))
                    {
                        srcProperty = srcProperties[k];
                        break;
                    }
                }

                if (srcProperty) {
                    Processor* srcProcessor = dynamic_cast<Processor*>(srcProperty->getOwner());
                    Processor* dstProcessor = dynamic_cast<Processor*>(dstProperty->getOwner());
                    ProcessorLink* processorLink = processorNetwork_->getLink(srcProcessor, dstProcessor);

                    if (!processorLink) {
                        addLink(srcProcessor, dstProcessor);
                        processorLink = processorNetwork_->getLink(srcProcessor, dstProcessor);
                    }

                    processorLink->addPropertyLinks(srcProperty, dstProperty);
                    processorLink->addPropertyLinks(dstProperty, srcProperty);
                }
            }
        }
    }
}



////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING PROCESSORS   //
////////////////////////////////////////////////////////
void NetworkEditor::addProcessorRepresentations(Processor* processor, QPointF pos, bool showProcessor, bool selectProcessor, bool showPropertyWidgets,
        bool showProcessorWidget) {
    // generate GUI representations (graphics item, property widget, processor widget)
    addProcessorGraphicsItem(processor, pos, showProcessor, selectProcessor);

    if (showPropertyWidgets)
        addPropertyWidgets(processor);

    addProcessorWidget(processor, showProcessorWidget);
    // TODO: Generalize by registering output/end processors (can also be e.g. VolumeSave)
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

    if (canvasProcessor)
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());
}

void NetworkEditor::removeProcessorRepresentations(Processor* processor) {
    // deregister processors which act as initiation points for the network evaluation
    // TODO: generalize, should be done for all output/end processors
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

    if (canvasProcessor)
        processorNetworkEvaluator_->deregisterCanvas(canvasProcessor->getCanvas());

    removeProcessorGraphicsItem(processor);
    removeAndDeletePropertyWidgets(processor);
    // processor widget should be removed here since it is added in addProcessorRepresentations()
    removeProcessorWidget(processor);
}

void NetworkEditor::addProcessorGraphicsItem(Processor* processor, QPointF pos, bool visible, bool selected) {
    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem();
    processorGraphicsItem->setProcessor(processor);
    processorGraphicsItem->setVisible(visible);
    processorGraphicsItem->setSelected(selected);

    // TODO: if (!sceneRect().contains(pos)) CLAMP_TO_SCENE_RECT;
    if (gridSnapping_) pos = snapToGrid(pos);

    processorGraphicsItem->setPos(pos);
    processorGraphicsItem->updateMetaData();
    addItem(processorGraphicsItem);
    processorGraphicsItems_.push_back(processorGraphicsItem);
}

void NetworkEditor::removeProcessorGraphicsItem(Processor* processor) {
    // obtain processor graphics item through processor
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(processor->getIdentifier());
    ivwAssert(processorGraphicsItem, "ProcessorGraphicsItem to be removed does not exist.");

    if (oldProcessorTarget_ == processorGraphicsItem)
        oldProcessorTarget_ = NULL;

    // remove connection graphics items
    std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = connectionGraphicsItems_;

    for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
        if (connectionGraphicsItems[i]->getOutProcessor() == processorGraphicsItem ||
            connectionGraphicsItems[i]->getInProcessor() == processorGraphicsItem)
            removeConnectionGraphicsItem(connectionGraphicsItems[i]);
    }

    // remove link graphics items
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems = linkGraphicsItems_;

    for (size_t i=0; i<linkGraphicsItems.size(); i++) {
        if (linkGraphicsItems[i]->getSrcProcessorGraphicsItem() == processorGraphicsItem ||
            linkGraphicsItems[i]->getDestProcessorGraphicsItem() == processorGraphicsItem)
            removeLinkGraphicsItem(linkGraphicsItems[i]);
    }

    // remove processor graphics item from editor
    processorGraphicsItem->hide();
    processorGraphicsItems_.erase(std::remove(processorGraphicsItems_.begin(), processorGraphicsItems_.end(),
                                  processorGraphicsItem), processorGraphicsItems_.end());
    // delete processor graphics item
    delete processorGraphicsItem;
}

void NetworkEditor::addPropertyWidgets(Processor* processor) {
    PropertyListWidget::instance()->addProcessorProperties(processor);
}

void NetworkEditor::removePropertyWidgets(Processor* processor) {
    PropertyListWidget::instance()->removeProcessorProperties(processor);
}

void NetworkEditor::removeAndDeletePropertyWidgets(Processor* processor) {
    PropertyListWidget::instance()->removeAndDeleteProcessorProperties(processor);
}

// remove processor widget unnecessary as processor widget is removed when processor is destroyed
void NetworkEditor::addProcessorWidget(Processor* processor, bool visible) {
    ProcessorWidgetQt* processorWidgetQt = dynamic_cast<ProcessorWidgetQt*>(ProcessorWidgetFactory::getRef().create(processor));

    if (processorWidgetQt) {
        processorWidgetQt->setProcessor(processor);
        InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
        processorWidgetQt->setParent(app->getMainWindow(), app->getMainWindow()->windowFlags());
        processor->setProcessorWidget(processorWidgetQt);
        processor->getProcessorWidget()->initialize();
        //TODO: Serialize if visible and check this on network load
        processor->getProcessorWidget()->setVisible(visible);
    }
}

void NetworkEditor::removeProcessorWidget(Processor* processor) {
    ProcessorWidgetQt* processorWidget = dynamic_cast<ProcessorWidgetQt*>(processor->getProcessorWidget());

    if (processorWidget) {
        processorWidget->deinitialize();
        processor->setProcessorWidget(NULL);
        delete processorWidget;
    }
}


/////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING CONNECTIONS   //
/////////////////////////////////////////////////////////
void NetworkEditor::removeConnection(ConnectionGraphicsItem* connectionGraphicsItem) {
    Outport* outport = connectionGraphicsItem->getOutport();
    Inport* inport = connectionGraphicsItem->getInport();
    removeConnectionGraphicsItem(connectionGraphicsItem);
    processorNetwork_->removeConnection(outport, inport);
}

void NetworkEditor::addConnectionGraphicsItem(Outport* outport, Inport* inport) {
    ProcessorGraphicsItem* processorGraphicsItem1 = getProcessorGraphicsItem(outport->getProcessor()->getIdentifier());
    ProcessorGraphicsItem* processorGraphicsItem2 = getProcessorGraphicsItem(inport->getProcessor()->getIdentifier());
    ConnectionGraphicsItem* connectionGraphicsItem = new ConnectionGraphicsItem(processorGraphicsItem1, outport,
            processorGraphicsItem2, inport);
    connectionGraphicsItems_.push_back(connectionGraphicsItem);
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->setVisible(processorGraphicsItem1->isVisible() && processorGraphicsItem2->isVisible());
}

void NetworkEditor::removeConnectionGraphicsItem(ConnectionGraphicsItem* connectionGraphicsItem) {
    if (oldConnectionTarget_ == connectionGraphicsItem)
        oldConnectionTarget_ = NULL;

    connectionGraphicsItem->hide();
    removeItem(connectionGraphicsItem);
    connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(),
                                   connectionGraphicsItem), connectionGraphicsItems_.end());
    delete connectionGraphicsItem;
}



//////////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING/DISPLAYING LINKS   //
//////////////////////////////////////////////////////////////
void NetworkEditor::removeLink(LinkConnectionGraphicsItem* linkGraphicsItem) {
    Processor* processor1 = linkGraphicsItem->getSrcProcessorGraphicsItem()->getProcessor();
    Processor* processor2 = linkGraphicsItem->getDestProcessorGraphicsItem()->getProcessor();
    removeLinkGraphicsItem(linkGraphicsItem);
    processorNetwork_->removeLink(processor1, processor2);
}

void NetworkEditor::addLinkGraphicsItem(Processor* processor1, Processor* processor2) {
    std::string linkInfo = processorNetwork_->getLink(processor1, processor2)->getLinkInfo();
    ProcessorGraphicsItem* processorGraphicsItem1 = getProcessorGraphicsItem(processor1->getIdentifier());
    ProcessorGraphicsItem* processorGraphicsItem2 = getProcessorGraphicsItem(processor2->getIdentifier());
    LinkConnectionGraphicsItem* linkGraphicsItem = new LinkConnectionGraphicsItem(processorGraphicsItem1,
            processorGraphicsItem2);

    if (!linkInfo.empty())
        linkGraphicsItem->setToolTip(QString(linkInfo.c_str()));

    linkGraphicsItems_.push_back(linkGraphicsItem);
    addItem(linkGraphicsItem);
    linkGraphicsItem->setVisible(processorGraphicsItem1->isVisible() && processorGraphicsItem2->isVisible());
    updateLinkGraphicsItems();
}

void NetworkEditor::removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem) {
    linkGraphicsItem->hide();
    removeItem(linkGraphicsItem);
    linkGraphicsItems_.erase(std::remove(linkGraphicsItems_.begin(), linkGraphicsItems_.end(), linkGraphicsItem),
                             linkGraphicsItems_.end());
    delete linkGraphicsItem;
}

void NetworkEditor::showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem) {
    Processor* srcProcessor = linkConnectionGraphicsItem->getSrcProcessorGraphicsItem()->getProcessor();
    Processor* destProcessor = linkConnectionGraphicsItem->getDestProcessorGraphicsItem()->getProcessor();
    LinkDialog linkDialog(srcProcessor, destProcessor, processorNetwork_, 0);
    linkDialog.exec();
    ProcessorLink* processorLink = processorNetwork_->getLink(srcProcessor, destProcessor);

    if (!processorLink->getPropertyLinks().size())
        removeLink(srcProcessor, destProcessor);
    else {
        std::string toolTip = processorLink->getLinkInfo();

        if (!toolTip.empty())
            linkConnectionGraphicsItem->setToolTip(QString(toolTip.c_str()));
    }
}

void  NetworkEditor::updateLinkGraphicsItems() {
    if (isLinkDisplayEnabled()) {
        for (size_t i=0; i<linkGraphicsItems_.size(); i++) {
            linkGraphicsItems_[i]->setVisible(true);
            ProcessorGraphicsItem* processorGraphicsItem1 = linkGraphicsItems_[i]->getSrcProcessorGraphicsItem();
            ProcessorGraphicsItem* processorGraphicsItem2 = linkGraphicsItems_[i]->getDestProcessorGraphicsItem();
            linkGraphicsItems_[i]->setVisible(processorGraphicsItem1->isVisible() && processorGraphicsItem2->isVisible());
        }
    }
    else {
        for (size_t i=0; i<linkGraphicsItems_.size(); i++)
            linkGraphicsItems_[i]->setVisible(false);
    }
}


//////////////////////////////////////
//   PORT INSPECTOR FUNCTIONALITY   //
//////////////////////////////////////

void NetworkEditor::managePortInspectors() {
    QPointF pos(inspection_.pos_.x, inspection_.pos_.y);
    Port* port = 0;
    ProcessorGraphicsItem* processor = getProcessorGraphicsItemAt(pos);
    ConnectionGraphicsItem* connection = getConnectionGraphicsItemAt(pos);

    if (processor) {
        port = processor->getSelectedPort(pos);
        // If we hover over an inport get its connected outport instead
        Inport* inport = dynamic_cast<Inport*>(port);

        if (inport)
            port = inport->getConnectedOutport();
    } else if (connection)
        port = connection->getOutport();

    if (!port) { // return to start
        removePortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_);
        hoverTimer_.stop();
        inspection_.resetPort();
        inspection_.setState(Inspection::Start);
    }

    if (inspection_.state() == Inspection::Start) {
        if (port) {
            inspection_.setState(Inspection::Wait);
            inspection_.setPort(port);
            hoverTimer_.start(500);
        }
    } else if (inspection_.state() == Inspection::Wait) {
        if (inspection_.samePort(port)) {
            if (hoverTimer_.isActive()) {
                //Keep waiting
            } else {
                // add port inspector
                inspection_.setState(Inspection::Inspect);
                addPortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_, QCursor::pos());
            }
        } else { // Left port before time out, reset.
            hoverTimer_.stop();
            inspection_.resetPort();
            inspection_.setState(Inspection::Start);
        }
    } else if (inspection_.state() == Inspection::Inspect) {
        if (inspection_.samePort(port)) {
            // Keep showing inspector
        } else {
            removePortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_);
            inspection_.setState(Inspection::Start);
            inspection_.resetPort();
        }
    }
}

void NetworkEditor::addPortInspector(std::string processorIdentifier, std::string portIdentifier, QPointF pos) {
    Processor* processor = processorNetwork_->getProcessorByName(processorIdentifier);

    if (!processor)
        return;

    Outport* port = processor->getOutport(portIdentifier);

    if (!port)
        return;

    PortInspector* portInspector =
        PortInspectorFactory::getPtr()->getPortInspectorForPortClass(port->getClassName());

    if (portInspector && !portInspector->isActive()) {
        portInspector->setActive(true);
        processorNetwork_->lock();
        processorNetwork_->setBroadcastModification(false);
        CanvasProcessor* canvasProcessor = portInspector->getCanvasProcessor();
        // Add processors to the network
        std::vector<Processor*> processors = portInspector->getProcessors();

        for (size_t i=0; i<processors.size(); i++) {
            processorNetwork_->addProcessor(processors[i]);
            //addProcessor(processors[i], QPointF(pos.x()+50*i, pos.y()), true, true, false);
        }

        addProcessorRepresentations(canvasProcessor, pos, false, false, false, false);
        // Add connections to the network
        std::vector<PortConnection*> connections = portInspector->getConnections();

        for (size_t i=0; i<connections.size(); i++) {
            processorNetwork_->addConnection(connections[i]->getOutport(),
                                             connections[i]->getInport());
        }

        // Add links to the network
        std::vector<ProcessorLink*> links = portInspector->getProcessorLinks();

        for (size_t i=0; i<links.size(); i++) {
            ProcessorLink* link = processorNetwork_->addLink(links[i]->getSourceProcessor(),
                                  links[i]->getDestinationProcessor());
            std::vector<PropertyLink*> propertyLinks = links[i]->getPropertyLinks();

            for (size_t j=0; j<propertyLinks.size(); j++) {
                link->addPropertyLinks(propertyLinks[j]->getSourceProperty(),
                                       propertyLinks[j]->getDestinationProperty());
            }
        }

        //Do some autolinking, without adding gui stuff.
        std::vector<Processor*> existingProcessors = processorNetwork_->getProcessors();
        LinkSettings* linkSettings = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>();
        ProcessorLink* processorLink = NULL;

        for (size_t i = 0; i < processors.size(); i++) {
            Processor* addedProcessor = processors[i];
            std::vector<Property*> dstProperties = addedProcessor->getProperties();

            for (size_t i = 0; i < dstProperties.size(); i++) {
                Property* dstProperty = dstProperties[i];

                for (size_t j = 0; j < existingProcessors.size(); j++) {
                    if (existingProcessors[j] != addedProcessor) {
                        std::vector<Property*> srcProperties = existingProcessors[j]->getProperties();
                        Property* srcProperty = 0;

                        for (size_t k = 0; k < srcProperties.size(); k++) {
                            if (AutoLinker::canLink(srcProperties[k], dstProperty, DefaultLinkingCondition) &&
                                linkSettings->isLinkable(srcProperties[k]) &&
                                linkSettings->isLinkable(dstProperty)) {
                                srcProperty = srcProperties[k];
                                break;
                            }
                        }

                        if (srcProperty) {
                            processorLink = processorNetwork_->addLink(existingProcessors[j], addedProcessor);
                            processorLink->addPropertyLinks(srcProperty, dstProperty);
                            processorLink->addPropertyLinks(dstProperty, srcProperty);
                        }
                    }
                }
            }
        }

        // Setup the widget
        ProcessorWidgetQt* processorWidgetQt =
            dynamic_cast<ProcessorWidgetQt*>(canvasProcessor->getProcessorWidget());
        ivwAssert(processorWidgetQt, "Processor widget not found in inspector network.");
        processorWidgetQt->setMinimumSize(inspection_.size(), inspection_.size());
        processorWidgetQt->setMaximumSize(inspection_.size(), inspection_.size());
        processorWidgetQt->setWindowFlags(Qt::CustomizeWindowHint
                                          | Qt::Tool
                                          | Qt::WindowStaysOnTopHint
                                          //| Qt::FramelessWindowHint
                                         );
        processorWidgetQt->move(ivec2(pos.x(),pos.y()));
        processorWidgetQt->show();
        processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(),
                canvasProcessor->getIdentifier());
        // Connect the port to inspect to the inports of the inspector network
        Outport* outport = dynamic_cast<Outport*>(port);
        std::vector<Inport*> inports = portInspector->getInports();

        for (size_t i=0; i<inports.size(); i++)
            processorNetwork_->addConnection(outport, inports[i]);

        processorNetwork_->unlock();
        processorNetwork_->setBroadcastModification(true);
        processorNetwork_->modified();
    }
}

void NetworkEditor::removePortInspector(std::string processorIdentifier, std::string portIdentifier) {
    Processor* processor = processorNetwork_->getProcessorByName(processorIdentifier);

    if (!processor)
        return;

    Outport* port = processor->getOutport(portIdentifier);

    if (!port)
        return;

    PortInspector* portInspector =
        PortInspectorFactory::getPtr()->getPortInspectorForPortClass(port->getClassName());

    if (portInspector && portInspector->isActive()) {
        processorNetwork_->lock();
        processorNetwork_->setBroadcastModification(false);
        CanvasProcessor* canvasProcessor = portInspector->getCanvasProcessor();
        // Remove processors from the network
        removeProcessorRepresentations(canvasProcessor);
        std::vector<Processor*> processors = portInspector->getProcessors();

        for (size_t i=0; i<processors.size(); i++)
            processorNetwork_->removeProcessor(processors[i]);

        processorNetwork_->unlock();
        processorNetwork_->setBroadcastModification(true);
        portInspector->setActive(false);
    }
}


void NetworkEditor::workerThreadReset() {
    workerThread_ = NULL;
}

void NetworkEditor::workerThreadQuit() {
    if (workerThread_) {
        workerThread_->quit();
        workerThread_->wait();
        workerThreadReset();
    }
}

void NetworkEditor::cacheProcessorProperty(Processor* p) {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    if (std::find(processors.begin(), processors.end(), p) != processors.end()) {
        PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
        processorNetwork_->lock();
        propertyListWidget_->cacheProcessorPropertiesItem(p);
        processorNetwork_->unlock();
    }
}

////////////////////////////////////////////////////////
//   LOAD AND GET SNAPSHOT FROM EXTERNAL NETWORK      //
////////////////////////////////////////////////////////

void NetworkEditor::addExternalNetwork(std::string fileName, std::string identifierPrefix, ivec2 pos, unsigned int networkEditorFlags ,
                                       ivec2 canvasSize) {
    processorNetwork_->lock();
    processorNetwork_->setBroadcastModification(false);
    IvwDeserializer xmlDeserializer(fileName);
    ProcessorNetwork* processorNetwork = new ProcessorNetwork();
    processorNetwork->deserialize(xmlDeserializer);
    std::vector<Processor*> processors = processorNetwork->getProcessors();

    for (size_t i=0; i<processors.size(); i++) {
        Processor* processor = processors[i];
        std::string newIdentifier = identifierPrefix+"_"+processor->getIdentifier();
        addProcessor(processor, QPointF(pos.x, pos.y), false, false, false);
        processor->setIdentifier(obtainUniqueProcessorID(newIdentifier));
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

        if (canvasProcessor) {
            // show processor widget as tool window
            ProcessorWidgetQt* processorWidgetQt = dynamic_cast<ProcessorWidgetQt*>(processor->getProcessorWidget());
            ivwAssert(processorWidgetQt, "Processor widget not found in inspector network.");

            if (!(networkEditorFlags & NetworkEditor::UseOriginalCanvasSize)) {
                processorWidgetQt->setMinimumSize(canvasSize[0], canvasSize[1]);
                processorWidgetQt->setMaximumSize(canvasSize[0], canvasSize[1]);
            }

            processorWidgetQt->setWindowFlags(Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
            processorWidgetQt->move(pos);
            processorWidgetQt->show();
            processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(),
                    canvasProcessor->getIdentifier()); //register is required because identifier is modified.
        }
    }

    std::vector<PortConnection*> connections = processorNetwork->getConnections();

    for (size_t i=0; i<connections.size(); i++) {
        PortConnection* connection = connections[i];
        Outport* outport = connection->getOutport();
        Inport* inport = connection->getInport();
        // first remove the connection from the loaded network to avoid an already connected warning
        processorNetwork->removeConnection(outport, inport);
        addConnection(outport, inport);
    }

    std::vector<ProcessorLink*> links = processorNetwork->getLinks();

    for (size_t i=0; i<links.size(); i++) {
        addLink(links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        ProcessorLink* link = processorNetwork_->getLink(links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        std::vector<PropertyLink*> propertyLinks = links[i]->getPropertyLinks();

        for (size_t j=0; j<propertyLinks.size(); j++) {
            Property* srcProp = propertyLinks[j]->getSourceProperty();
            Property* dstProp = propertyLinks[j]->getDestinationProperty();
            link->addPropertyLinks(srcProp, dstProp);
        }
    }

    processorNetwork_->unlock();
    processors = processorNetwork->getProcessors();

    for (size_t i=0; i<processors.size(); i++) {
        Processor* processor = processors[i];

        if (processor->hasProcessorWidget())
            processor->getProcessorWidget()->show();
    }

    processorNetwork_->setBroadcastModification(true);
}

std::vector<std::string> NetworkEditor::saveSnapshotsInExternalNetwork(std::string externalNetworkFile, std::string identifierPrefix) {
    //turnoff sound
    BoolProperty* soundProperty = dynamic_cast<BoolProperty*>
                                  (InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("enableSound"));
    bool isSoundEnabled = soundProperty->get();

    if (isSoundEnabled) soundProperty->set(false);

    std::vector<std::string> canvasSnapShotFiles;
    std::string directory = URLParser::getFileDirectory(externalNetworkFile);
    std::string workSpaceName = URLParser::getFileNameWithExtension(externalNetworkFile);
    std::string newFileName = URLParser::replaceFileExtension(workSpaceName, "png");
    processorNetwork_->setBroadcastModification(false);
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    for (size_t i=0; i<processors.size(); i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix)!=std::string::npos) {
            CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processors[i]);

            if (canvasProcessor) {
                std::string snapShotFilePath = directory + "snapshot_" + canvasProcessor->getIdentifier() + newFileName;
                canvasSnapShotFiles.push_back(snapShotFilePath);
                canvasProcessor->saveImageLayer(snapShotFilePath.c_str());
            }
        }
    }

    if (isSoundEnabled) soundProperty->set(true);

    processorNetwork_->setBroadcastModification(true);
    return canvasSnapShotFiles;
}

void NetworkEditor::removeExternalNetwork(std::string identifierPrefix) {
    processorNetwork_->lock();
    processorNetwork_->setBroadcastModification(false);
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    for (size_t i=0; i<processors.size(); i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix)!=std::string::npos)
            removeProcessor(processors[i]);
    }

    processorNetwork_->unlock();
    processorNetwork_->setBroadcastModification(true);
}


std::vector<std::string> NetworkEditor::getSnapshotsOfExternalNetwork(std::string fileName) {
    std::vector<std::string> snapshotFileNames;
    //load external network
    QRectF rect = sceneRect();
    ivec2 pos(rect.width()/2, rect.height()/2);
    std::string identifierPrefix = "TemporaryExternalNetwork";
    unsigned int networkEditorFlags = NetworkEditor::UseOriginalCanvasSize | NetworkEditor::CanvasHidden;
    addExternalNetwork(fileName, identifierPrefix, pos, networkEditorFlags);
    processorNetwork_->setModified(true);
    processorNetworkEvaluator_->requestEvaluate();
    //save snapshot
    snapshotFileNames = saveSnapshotsInExternalNetwork(fileName, identifierPrefix);
    //unload external network
    removeExternalNetwork(identifierPrefix);
    return snapshotFileNames;
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

ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItem(Outport* outport, Inport* inport) const {
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        if (connectionGraphicsItems_[i]->getOutport() == outport &&
            connectionGraphicsItems_[i]->getInport() == inport)
            return connectionGraphicsItems_[i];

    return 0;
}

LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItem(Processor* processor1, Processor* processor2) const {
    for (size_t i=0; i<linkGraphicsItems_.size(); i++) {
        Processor* outProcessor = linkGraphicsItems_[i]->getSrcProcessorGraphicsItem()->getProcessor();
        Processor* inProcessor = linkGraphicsItems_[i]->getDestProcessorGraphicsItem()->getProcessor();

        if ((outProcessor == processor1 && inProcessor == processor2)  ||
            (outProcessor == processor2 && inProcessor == processor1))
            return linkGraphicsItems_[i];
    }

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
    hoverTimer_.stop();

    if (e->button() == Qt::LeftButton) {
        startProcessor_ = getProcessorGraphicsItemAt(e->scenePos());

        if (startProcessor_) {
            if (e->modifiers()==Qt::NoModifier) {
                startPort_ = startProcessor_->getSelectedPort(e->scenePos());

                if (startPort_ && dynamic_cast<Outport*>(startPort_)) {
                    // click on outport: start drawing a connection
                    QRectF portRect = startProcessor_->calculatePortRect(dynamic_cast<Outport*>(startPort_));
                    portRect = startProcessor_->mapToScene(portRect).boundingRect();
                    connectionCurve_ = new CurveGraphicsItem(portRect.center(), e->scenePos(), startPort_->getColorCode());
                    addItem(connectionCurve_);
                    connectionCurve_->show();
                    e->accept();
                    return;
                } else if (startPort_ && dynamic_cast<Inport*>(startPort_)) {
                    // click on inport: disconnect if connected
                    // FIXME: delete operation in release event handling results in a crash when
                    //        disconnecting two ports through drag and drop
                    if (startPort_->isConnected()) {
                        // first remove existing connection, and remember start port
                        std::vector<PortConnection*> portConnections = processorNetwork_->getConnections();

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
                        e->accept();
                        return;
                    }
                }  
                else
                    updateAllProcessorGraphicsItemMetaData();
            } else if (e->modifiers() == Qt::ControlModifier) {
                if (isLinkDisplayEnabled()) {
                    // ctrl modifier pressed: edit link
                    QRectF processorRect(0,0,0,0);
                    processorRect = startProcessor_->mapToScene(processorRect).boundingRect();
                    linkCurve_ = new LinkGraphicsItem(processorRect.center(), e->scenePos());
                    addItem(linkCurve_);
                    linkCurve_->show();
                    e->accept();
                    return;
                }
                else {
                    LogWarn("Enable Display links in Settings to create links")
                }

                e->accept();
            }
        }
    }

    QGraphicsScene::mousePressEvent(e);
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // Connection drag mode
        connectionCurve_->setEndPoint(e->scenePos());
        endProcessor_ = getProcessorGraphicsItemAt(e->scenePos());

        if (endProcessor_) {
            Port* port = endProcessor_->getSelectedPort(e->scenePos());

            if (port && port!=startPort_) {
                Inport* inport = dynamic_cast<Inport*>(port);

                if (inport && inport->canConnectTo(startPort_)
                    && !inport->isConnectedTo(dynamic_cast<Outport*>(startPort_)))
                    connectionCurve_->setBorderColor(Qt::green);
                else
                    connectionCurve_->setBorderColor(Qt::red);
            } else
                connectionCurve_->resetBorderColors();
        } else
            connectionCurve_->resetBorderColors();

        endProcessor_ = NULL;
        connectionCurve_->update();
        e->accept();
    } else if (linkCurve_) {
        // Link drag mode
        QPointF center = startProcessor_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        linkCurve_->update();
        e->accept();
    } else if (inspection_.isActive() && e->button() == Qt::NoButton) {
        // Port inspector hover effect
        inspection_.pos_ = ivec2(e->scenePos().x(),e->scenePos().y());
        managePortInspectors();
    }

    QGraphicsScene::mouseMoveEvent(e);
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

            if (endPort_) {
                Inport* inport = dynamic_cast<Inport*>(endPort_);

                if (inport && inport->canConnectTo(startPort_)) {
                    // MultiDataInports can have several connections
                    SingleInport* singleInport = dynamic_cast<SingleInport*>(inport);

                    if (inport->isConnected() && singleInport != NULL)
                        removeConnection(singleInport->getConnectedOutport(), inport);

                    addConnection(dynamic_cast<Outport*>(startPort_), inport);
                }
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
        endProcessor_ = getProcessorGraphicsItemAt(e->scenePos());

        if (startProcessor_!=endProcessor_ && endProcessor_) {
            addLink(startProcessor_->getProcessor(), endProcessor_->getProcessor());
            LinkConnectionGraphicsItem* linkGraphicsItem = getLinkGraphicsItem(startProcessor_->getProcessor(), endProcessor_->getProcessor());
            showLinkDialog(linkGraphicsItem);
        }

        startProcessor_ = 0;
        endProcessor_ = 0;
        e->accept();
    } else if (startProcessor_) {
        // move processor
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();

        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);

            if (processorGraphicsItem) {
                if (gridSnapping_)
                    processorGraphicsItem->setPos(snapToGrid(processorGraphicsItem->pos()));
                processorGraphicsItem->updateMetaData();
            }

        }

        updateAllProcessorGraphicsItemMetaData();

        QGraphicsScene::mouseReleaseEvent(e);
    } else {
        updateAllProcessorGraphicsItemMetaData();              
    }

    QGraphicsScene::mouseReleaseEvent(e);
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

            if (processorGraphicsItem && !processorGraphicsItem->isEditingProcessorName()) {
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
        processorGraphicsItem->setSelected(true);
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

        if (result == renameAction)
            processorGraphicsItem->editProcessorName();
        else if (result == deleteAction) {
            Processor* processor = processorGraphicsItem->getProcessor();
            removeProcessor(processor);
        } else if (showAction && result == showAction) {
            if (showAction->isChecked())
                processorGraphicsItem->getProcessor()->getProcessorWidget()->show();
            else
                processorGraphicsItem->getProcessor()->getProcessorWidget()->hide();
        }
    } else if (connectionGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction(tr("Delete"));
        QAction* result = menu.exec(QCursor::pos());

        if (result == action)
            removeConnection(connectionGraphicsItem);
    } else if (linkGraphicsItem) {
        if (isLinkDisplayEnabled()) {
            QMenu menu;
            QAction* linkAction = menu.addAction(tr("Link properties"));
            QAction* deleteAction = menu.addAction("Delete");
            QAction* result = menu.exec(QCursor::pos());

            if (result == deleteAction)
                removeLink(linkGraphicsItem);
            else if (result == linkAction) {
                Processor* inProcessor = linkGraphicsItem->getDestProcessorGraphicsItem()->getProcessor();
                Processor* outProcessor = linkGraphicsItem->getSrcProcessorGraphicsItem()->getProcessor();
                ProcessorLink* processorLink = processorNetwork_->getLink(inProcessor, outProcessor);

                if (processorLink)
                    processorLink->autoLinkPropertiesByType();
            }
        }
    } else
        QGraphicsScene::contextMenuEvent(e);
}

void NetworkEditor::updateAllProcessorGraphicsItemMetaData() {
    QList<QGraphicsItem*> selectedGraphicsItems = items();
    for (int i=0; i<selectedGraphicsItems.size(); i++) {
        ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
        if (processorGraphicsItem)
            processorGraphicsItem->updateMetaData();                
    }  
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
        ConnectionGraphicsItem* connectionItem = getConnectionGraphicsItemAt(e->scenePos());

        if (connectionItem && !oldConnectionTarget_) { //< New connection found
            QString className;
            ProcessorDragObject::decode(e->mimeData(), className);
            Processor* processor = static_cast<Processor*>(ProcessorFactory::getRef().create(className.toLocal8Bit().constData()));
            QColor inputColor = Qt::red, outputColor = Qt::red;

            // try to find a match between connection outport and one of the new processors inports
            for (size_t i = 0; i < processor->getInports().size(); ++i) {
                if (processor->getInports().at(i)->canConnectTo(connectionItem->getOutport())) {
                    inputColor = Qt::green;
                    break;
                }
            }

            // try to find a match between connection inport and one of the new processors outports
            for (size_t i = 0; i < processor->getOutports().size(); ++i) {
                if (connectionItem->getInport()->canConnectTo(processor->getOutports().at(i))) {
                    outputColor = Qt::green;
                    break;
                }
            }

            connectionItem->setBorderColors(inputColor, outputColor);
            oldConnectionTarget_ = connectionItem;
            delete processor;
        } else if (connectionItem)   // move event on active connection
            oldConnectionTarget_->setMidPoint(e->scenePos());
        else if (oldConnectionTarget_ && !connectionItem) { //< Connection no longer targeted
            oldConnectionTarget_->clearMidPoint();
            oldConnectionTarget_ = NULL;
        } else if (!connectionItem) { // processor replacement
            ProcessorGraphicsItem* processorItem = getProcessorGraphicsItemAt(e->scenePos());

            if (processorItem && !oldProcessorTarget_) { //< New processor found
                QString className;
                ProcessorDragObject::decode(e->mimeData(), className);
                processorItem->setSelected(true);
                oldProcessorTarget_ = processorItem;
            } else if (!processorItem && oldProcessorTarget_) {  // processor no longer targeted
                oldProcessorTarget_->setSelected(false);
                oldProcessorTarget_ = NULL;
            }
        }
    }
}

void NetworkEditor::dropEvent(QGraphicsSceneDragDropEvent* e) {
    if (ProcessorDragObject::canDecode(e->mimeData())) {
        QString className;
        ProcessorDragObject::decode(e->mimeData(), className);

        if (!className.isEmpty()) {
            // create processor, add it to processor network, and generate it's widgets
            Processor* processor = static_cast<Processor*>(ProcessorFactory::getRef().create(className.toLocal8Bit().constData()));
            addProcessor(processor, e->scenePos());
            ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(processor->getIdentifier());
            clearSelection();

            if (processorGraphicsItem)
                processorGraphicsItem->setSelected(true);

            e->setAccepted(true);
            e->acceptProposedAction();

            // check for collisions
            if (oldConnectionTarget_)
                placeProcessorOnConnection(processorGraphicsItem, oldConnectionTarget_);
            else if (oldProcessorTarget_)
                placeProcessorOnProcessor(processorGraphicsItem, oldProcessorTarget_);
        }
    }
}

void NetworkEditor::placeProcessorOnConnection(ProcessorGraphicsItem* processorItem, ConnectionGraphicsItem* connectionItem) {
    std::vector<Inport*> inports = processorItem->getProcessor()->getInports();
    std::vector<Outport*> outports = processorItem->getProcessor()->getOutports();
    Inport* connectionInport = connectionItem->getInport();
    Outport* connectionOutport = connectionItem->getOutport();
    // clear oldDragTarget
    oldConnectionTarget_->clearMidPoint();
    oldConnectionTarget_ = NULL;
    processorNetwork_->lock();
    // remove old connection
    removeConnection(connectionItem);

    for (size_t i = 0; i < inports.size(); ++i) {
        if (inports.at(i)->canConnectTo(connectionOutport)) {
            // create new connection connectionOutport-processorInport
            addConnection(connectionOutport, inports.at(i));
            break;
        }
    }

    for (size_t i = 0; i < outports.size(); ++i) {
        if (connectionInport->canConnectTo(outports.at(i))) {
            // create new connection processorOutport-connectionInport
            addConnection(outports.at(i), connectionInport);
            break;
        }
    }

    processorNetwork_->unlock();

    if (processorNetwork_->isModified())
        processorNetworkEvaluator_->requestEvaluate();
}

void NetworkEditor::placeProcessorOnProcessor(ProcessorGraphicsItem* processorItem, ProcessorGraphicsItem* oldProcessorItem) {
    const std::vector<Inport*>& inports = processorItem->getProcessor()->getInports();
    const std::vector<Outport*>& outports = processorItem->getProcessor()->getOutports();
    const std::vector<Inport*>& oldInports = oldProcessorItem->getProcessor()->getInports();
    const std::vector<Outport*>& oldOutports = oldProcessorItem->getProcessor()->getOutports();
    processorNetwork_->lock();
    std::vector<std::pair<Outport*, Inport*> > newConnections;

    for (size_t i = 0; i < std::min(inports.size(), oldInports.size()); ++i) {
        if (inports.at(i)->canConnectTo(oldInports.at(i)->getConnectedOutport())) {
            // save new connection connectionOutportoldInport-processorInport
            newConnections.push_back(std::make_pair(oldInports.at(i)->getConnectedOutport(), inports.at(i)));
        }
    }

    for (size_t i = 0; i < std::min(outports.size(), oldOutports.size()); ++i) {
        std::vector<Inport*> connectionInports = oldOutports.at(i)->getConnectedInports();

        for (size_t j = 0; j < connectionInports.size(); ++j) {
            if (connectionInports.at(j)->canConnectTo(outports.at(i))) {
                // save new connection processorOutport-connectionInport
                newConnections.push_back(std::make_pair(outports.at(i), connectionInports.at(j)));
            }
        }
    }

    // remove old processor
    removeProcessor(oldProcessorItem->getProcessor());

    // create all new connections
    for (size_t i = 0; i < newConnections.size(); ++i)
        addConnection(newConnections.at(i).first, newConnections.at(i).second);

    processorNetwork_->unlock();

    if (processorNetwork_->isModified())
        processorNetworkEvaluator_->requestEvaluate();
}

///////////////////////////////
//   SERIALIZATION METHODS   //
///////////////////////////////
void NetworkEditor::clearNetwork() {
    processorNetwork_->lock();
    ResourceManager::instance()->clearAllResources();
    
    removePortInspector(inspection_.processorIdentifier_, inspection_.portIdentifier_);
    
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        removeProcessor(processors[i]);

    processorNetwork_->unlock();
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    IvwSerializer xmlSerializer(fileName);
    processorNetwork_->serialize(xmlSerializer);
    processorNetwork_->setModified(false);
    xmlSerializer.writeFile();
    filename_ = fileName;
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {
    workerThreadQuit();
    // first we clean the current network
    clearNetwork();
    // then we lock the network that no evaluations are triggered during the deserialization
    processorNetwork_->lock();

    // then we deserialize processor network
    try {
        IvwDeserializer xmlDeserializer(fileName);
        processorNetwork_->deserialize(xmlDeserializer);
    }
    catch (const AbortException& exception) {
        LogInfo("Unable to load network " + fileName + " due to " + exception.getMessage());
        clearNetwork();
        processorNetwork_->unlock();
        return false;
    }
    catch (const IgnoreException& exception) {
        LogInfo("Incomplete network loading " + fileName + " due to " + exception.getMessage());
    }

    // add processors
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    for (size_t i=0; i<processors.size(); i++) {
        processors[i]->invalidate(PropertyOwner::INVALID_RESOURCES);
        ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));
        addProcessorRepresentations(processors[i], QPointF(meta->getPosition().x, meta->getPosition().y), meta->isVisible(), meta->isSelected(), false, false);
    }

    // add connections
    std::vector<PortConnection*> connections = processorNetwork_->getConnections();

    for (size_t i=0; i<connections.size(); i++)
        addConnectionGraphicsItem(connections[i]->getOutport(), connections[i]->getInport());

    // add link graphics items
    std::vector<ProcessorLink*> links = processorNetwork_->getLinks();

    for (size_t i=0; i<links.size(); i++)
        addLinkGraphicsItem(links[i]->getDestinationProcessor(), links[i]->getSourceProcessor());

    // flag the network's modified flag
    processorNetwork_->setModified(true);

    // show all processor widgets that where hidden on network load
    for (size_t i=0; i<processors.size(); i++) {
        if (processors[i]->hasProcessorWidget()) {
            ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));

            if (meta->isVisible())
                processors[i]->getProcessorWidget()->show();
        }
    }

    // unlock it and initiate evaluation
    processorNetwork_->unlock();
    processorNetwork_->modified();
    // create all property (should be all non-visible) widgets in a thread (as it can take a long time to create them)
    workerThread_ = new QThread();
    ProcessorWorkerQt* worker = new ProcessorWorkerQt(processors);
    worker->moveToThread(workerThread_);
    connect(workerThread_, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(nextProcessor(Processor*)), this, SLOT(cacheProcessorProperty(Processor*)));
    connect(worker, SIGNAL(finished()), workerThread_, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(workerThread_, SIGNAL(finished()), workerThread_, SLOT(deleteLater()));
    connect(workerThread_, SIGNAL(finished()), this, SLOT(workerThreadReset()));
    workerThread_->start();

    for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
        static_cast<NetworkEditorObserver*>(*it)->onNetworkEditorFileChanged(fileName);

    filename_ = fileName;
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

void NetworkEditor::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->setWorldMatrixEnabled(true);
    painter->fillRect(rect, Qt::darkGray);
    qreal left = int(rect.left()) - (int(rect.left()) % GRID_SPACING);
    qreal top = int(rect.top()) - (int(rect.top()) % GRID_SPACING);
    QVarLengthArray<QLineF, 100> linesX;

    for (qreal x = left; x < rect.right(); x += GRID_SPACING)
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;

    for (qreal y = top; y < rect.bottom(); y += GRID_SPACING)
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}

std::string NetworkEditor::obtainUniqueProcessorID(std::string identifierPrefix) const {
    if (!processorNetwork_->getProcessorByName(identifierPrefix))
        return identifierPrefix;

    unsigned int idNumber = 1;
    std::string validProcessorID;
    validProcessorID = identifierPrefix;

    do {
        std::stringstream stringStream;
        stringStream << idNumber++;
        validProcessorID = identifierPrefix + stringStream.str();
    } while (processorNetwork_->getProcessorByName(validProcessorID));

    return validProcessorID;
}

bool NetworkEditor::isLinkDisplayEnabled() {
    Property* prop = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>()->getPropertyByIdentifier("displayLinks");
    ivwAssert(prop!=0, "Display Links property not found in settings");
    BoolProperty* displayLinkProperty = dynamic_cast<BoolProperty*>(prop);
    return displayLinkProperty->get();
}

void ProcessorWorkerQt::process() {
    for (std::vector<Processor*>::iterator it = processors_.begin(); it != processors_.end(); ++it)
        emit nextProcessor(*it);

    emit finished();
}

} // namespace
