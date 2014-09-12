/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Timo Ropinski, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/editor/networkeditor.h>

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/multidatainport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/ports/portinspectorfactory.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/resources/resourcemanager.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/settings/linksettings.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/util/urlparser.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>
#include <inviwo/qt/editor/linkdialog.h>
#include <inviwo/qt/editor/linkgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/editor/processorlinkgraphicsitem.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/processorportgraphicsitem.h>
#include <inviwo/qt/editor/processorprogressgraphicsitem.h>
#include <inviwo/qt/editor/processorstatusgraphicsitem.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <inviwo/qt/widgets/propertylistwidget.h>

#include <QApplication>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>


namespace inviwo {

const int NetworkEditor::GRID_SPACING = 25;

NetworkEditor::NetworkEditor()
    : QGraphicsScene()
    , oldConnectionTarget_(NULL)
    , oldProcessorTarget_(NULL)
    , connectionCurve_(NULL)
    , linkCurve_(NULL)
    , filename_("")
    , modified_(false)
    , cacheProcessorPropertyDoneEventId_(-1)
    , markModifedFlaseEventId_(-1)
    , portInspectorProcessorIdentifier_("")
    , portInspectorPortIdentifier_("") {
    InviwoApplication::getPtr()->getProcessorNetwork()->addObserver(this);
    cacheProcessorPropertyDoneEventId_ = QEvent::registerEventType();
    markModifedFlaseEventId_ = QEvent::registerEventType();


    // The defalt bsp tends to crash...  
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

NetworkEditor::~NetworkEditor() {}

/////////////////////////////////////////////
//   PUBLIC METHODS FOR CHANGING NETWORK   //
/////////////////////////////////////////////
ProcessorGraphicsItem* NetworkEditor::addProcessor(Processor* processor, QPointF pos,
                                                   bool showProcessor, bool selectProcessor,
                                                   bool showPropertyWidgets,
                                                   bool showProcessorWidget) {
    ProcessorGraphicsItem* ret = NULL;
    // add the processor to the network
    processor->initialize();
    InviwoApplication::getPtr()->getProcessorNetwork()->addProcessor(processor);
    // add processor representations
    ret = addProcessorRepresentations(processor, pos, showProcessor, showPropertyWidgets,
                                      selectProcessor);
    autoLinkOnAddedProcessor(processor);
    setModified(true);
    return ret;
}

void NetworkEditor::removeProcessor(Processor* processor) {
    removePortInspector();

    // remove processor representations
    removeProcessorRepresentations(processor);
    // enable default rendering context before deinitializing processor
    InviwoApplication::getPtr()->getProcessorNetworkEvaluator()->activateDefaultRenderContext();
    // remove the processor from the network
    InviwoApplication::getPtr()->getProcessorNetwork()->removeAndDeleteProcessor(processor);
    // set network to modified
    setModified(true);
}

ConnectionGraphicsItem* NetworkEditor::addConnection(Outport* outport, Inport* inport) {
    PortConnection* connection =
        InviwoApplication::getPtr()->getProcessorNetwork()->addConnection(outport, inport);
    ConnectionGraphicsItem* ret = NULL;
    if (connection != NULL) {
        ret = addConnectionGraphicsItem(connection);
        setModified(true);
    }

    updateLeds();

    return ret;
}

void NetworkEditor::removeConnection(Outport* outport, Inport* inport) {
    PortConnection* connection =
        InviwoApplication::getPtr()->getProcessorNetwork()->getConnection(outport, inport);
    removeConnectionGraphicsItem(connection);
    InviwoApplication::getPtr()->getProcessorNetwork()->removeConnection(outport, inport);

    updateLeds();

    setModified(true);
}

LinkConnectionGraphicsItem* NetworkEditor::addLink(PropertyOwner* processor1,
                                                   PropertyOwner* processor2) {
    LinkConnectionGraphicsItem* ret = NULL;
    ProcessorLink* link = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLink(
        processor1, processor2);

    Processor* src = dynamic_cast<Processor*>(processor1);
    Processor* dst = dynamic_cast<Processor*>(processor2);

    if (!link) {
        InviwoApplication::getPtr()->getProcessorNetwork()->addLink(processor1, processor2);
        ret = addLinkGraphicsItem(src, dst);
        setModified(true);
    } else {
        ret = getLinkGraphicsItem(src, dst);
    }
    return ret;
}

void NetworkEditor::removeLink(PropertyOwner* processor1, PropertyOwner* processor2) {
    Processor* src = dynamic_cast<Processor*>(processor1);
    Processor* dst = dynamic_cast<Processor*>(processor2);
    removeLinkGraphicsItem(getLinkGraphicsItem(src, dst));
    InviwoApplication::getPtr()->getProcessorNetwork()->removeLink(processor1, processor2);
    setModified(true);
}

void NetworkEditor::autoLinkOnAddedProcessor(Processor* addedProcessor) {
    std::vector<Processor*> existingProcessors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
    std::vector<Property*> dstProperties = addedProcessor->getProperties();
    LinkSettings* linkSettings = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>();
    std::vector<Processor*> invalidProcessors;

    for (size_t i = 0; i < dstProperties.size(); i++) {
        Property* dstProperty = dstProperties[i];

        for (size_t j = 0; j < existingProcessors.size(); j++) {
            if (existingProcessors[j] != addedProcessor) {
                std::vector<Property*> srcProperties = existingProcessors[j]->getProperties();

                std::vector<PropertyLink*> newLinks;

                if (linkSettings->isLinkable(dstProperty)){
                    for (size_t k = 0; k < srcProperties.size(); k++) if (linkSettings->isLinkable(srcProperties[k])) {
                        if (AutoLinker::canLink(srcProperties[k], dstProperty, DefaultLinkingCondition)) {
                            Property* srcProperty = srcProperties[k];
                            newLinks.push_back(
                                InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                                srcProperty, dstProperty));
                            newLinks.push_back(
                                InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                                dstProperty, srcProperty));
                        }
                    }
                }

                if (!newLinks.empty()) {
                    if (std::find(invalidProcessors.begin(), invalidProcessors.end(),
                                  existingProcessors[j]) == invalidProcessors.end()) {
                        invalidProcessors.push_back(existingProcessors[j]);
                    }

                    addLink(existingProcessors[j], addedProcessor);
                    ProcessorLink* link =
                        InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                            existingProcessors[j], addedProcessor);
                    for (size_t ii = 0; ii < newLinks.size(); ii++)
                        link->addPropertyLinks(newLinks[ii]);
                }
            }
        }
    }
    if (invalidProcessors.size()) {
        InviwoApplication::getPtr()->getProcessorNetwork()->lock();
        for (size_t i = 0; i < invalidProcessors.size(); i++)
            InviwoApplication::getPtr()->getProcessorNetwork()->setLinkModifiedByOwner(
                invalidProcessors[i]);
        InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
        InviwoApplication::getPtr()->getProcessorNetwork()->setLinkModifiedByOwner(
            invalidProcessors[0]);
    }
}

////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING PROCESSORS   //
////////////////////////////////////////////////////////
ProcessorGraphicsItem* NetworkEditor::addProcessorRepresentations(Processor* processor, QPointF pos,
                                                                  bool showProcessor,
                                                                  bool selectProcessor,
                                                                  bool showPropertyWidgets,
                                                                  bool showProcessorWidget) {
    // generate GUI representations (graphics item, property widget, processor widget)
    ProcessorGraphicsItem* ret =
        addProcessorGraphicsItem(processor, pos, showProcessor, selectProcessor);

    // If the processor is selected the widgets will be added anyway
    if (showPropertyWidgets && !selectProcessor) addPropertyWidgets(processor);

    addProcessorWidget(processor, false);  // showProcessorWidget);

    // TODO: Generalize by registering output/end processors (can also be e.g. VolumeSave)
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
    if (canvasProcessor) {
        InviwoApplication::getPtr()->getProcessorNetworkEvaluator()->registerCanvas(
            canvasProcessor->getCanvas(), canvasProcessor->getIdentifier());
    }

    if (processor->hasProcessorWidget()) {
        processor->getProcessorWidget()->setVisible(showProcessorWidget);
        processor->getProcessorWidget()->addObserver(ret->getStatusItem());
    }

    return ret;
}

void NetworkEditor::removeProcessorRepresentations(Processor* processor) {
    // deregister processors which act as initiation points for the network evaluation
    // TODO: generalize, should be done for all output/end processors
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

    if (canvasProcessor)
        InviwoApplication::getPtr()->getProcessorNetworkEvaluator()->deregisterCanvas(
            canvasProcessor->getCanvas());

    removeProcessorGraphicsItem(processor);
    removeAndDeletePropertyWidgets(processor);
    // processor widget should be removed here since it is added in addProcessorRepresentations()
    removeProcessorWidget(processor);
}

ProcessorGraphicsItem* NetworkEditor::addProcessorGraphicsItem(Processor* processor, QPointF pos,
                                                               bool visible, bool selected) {
    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem(processor);
    processorGraphicsItem->setVisible(visible);
    processorGraphicsItem->setSelected(selected);

    // TODO: if (!sceneRect().contains(pos)) CLAMP_TO_SCENE_RECT;
    processorGraphicsItem->setPos(pos);

    addItem(processorGraphicsItem);
    processorGraphicsItems_[processor] = processorGraphicsItem;

    return processorGraphicsItem;
}

void NetworkEditor::removeProcessorGraphicsItem(Processor* processor) {
    // obtain processor graphics item through processor

    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItem(processor);

    if (oldProcessorTarget_ == processorGraphicsItem) oldProcessorTarget_ = NULL;

    // remove connections involving the processor
    ConnectionMap cgi = connectionGraphicsItems_;

    for (ConnectionMap::const_iterator it = cgi.begin(); it != cgi.end(); it++) {
        if (it->second->getOutProcessor() == processorGraphicsItem ||
            it->second->getInProcessor() == processorGraphicsItem) {
            removeConnectionGraphicsItem(it->first);
        }
    }

    // remove link graphics items
    LinkMap lgi = linkGraphicsItems_;

    for (LinkMap::const_iterator it = lgi.begin(); it != lgi.end(); it++) {
        if (it->second->getSrcProcessorGraphicsItem() == processorGraphicsItem ||
            it->second->getDestProcessorGraphicsItem() == processorGraphicsItem)
            removeLinkGraphicsItem(it->second);
    }

    // remove processor graphics item from editor
   // processorGraphicsItem->hide();
    processorGraphicsItems_.erase(processor);
    // delete processor graphics item
    delete processorGraphicsItem;
}

void NetworkEditor::addPropertyWidgets(Processor* processor) {
    QCoreApplication::postEvent(
        PropertyListWidget::instance(),
        new PropertyListEvent(PropertyListEvent::ADD, processor->getIdentifier()),
        Qt::LowEventPriority);
}

void NetworkEditor::removePropertyWidgets(Processor* processor) {
    QCoreApplication::postEvent(
        PropertyListWidget::instance(),
        new PropertyListEvent(PropertyListEvent::REMOVE, processor->getIdentifier()),
        Qt::LowEventPriority);
}

void NetworkEditor::removeAndDeletePropertyWidgets(Processor* processor) {
    // Will not use events here since we might delete the processor
    PropertyListWidget::instance()->removeAndDeleteProcessorProperties(processor);
}

// remove processor widget unnecessary as processor widget is removed when processor is destroyed
void NetworkEditor::addProcessorWidget(Processor* processor, bool visible) {
    ProcessorWidget* processorWidget =
        dynamic_cast<ProcessorWidget*>(ProcessorWidgetFactory::getPtr()->create(processor));

    if (processorWidget) {
        processorWidget->setProcessor(processor);

        ProcessorWidgetQt* processorWidgetQt = dynamic_cast<ProcessorWidgetQt*>(processorWidget);
        if (processorWidgetQt) {
            InviwoApplicationQt* app =
                dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
            processorWidgetQt->setParent(app->getMainWindow(),
                                         Qt::Window | Qt::WindowStaysOnTopHint);
        }

        processor->setProcessorWidget(processorWidget);
        processor->getProcessorWidget()->initialize();
        // TODO: Serialize if visible and check this on network load
        processor->getProcessorWidget()->setVisible(visible);
    }
}

void NetworkEditor::removeProcessorWidget(Processor* processor) {
    ProcessorWidget* processorWidget = processor->getProcessorWidget();

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
    removeConnection(outport, inport);
}

ConnectionGraphicsItem* NetworkEditor::addConnectionGraphicsItem(PortConnection* connection) {
    Outport* outport = connection->getOutport();
    Inport* inport = connection->getInport();

    ProcessorGraphicsItem* outProcessor = getProcessorGraphicsItem(outport->getProcessor());
    ProcessorGraphicsItem* inProcessor = getProcessorGraphicsItem(inport->getProcessor());

    ConnectionGraphicsItem* connectionGraphicsItem =
        new ConnectionGraphicsItem(outProcessor->getOutportGraphicsItem(outport),
                                   inProcessor->getInportGraphicsItem(inport), connection);

    connectionGraphicsItems_[connection] = connectionGraphicsItem;
    addItem(connectionGraphicsItem);
    connectionGraphicsItem->setVisible(outProcessor->isVisible() && inProcessor->isVisible());

    return connectionGraphicsItem;
}

void NetworkEditor::removeConnectionGraphicsItem(PortConnection* connection) {
    ConnectionGraphicsItem* connectionGraphicsItem = connectionGraphicsItems_[connection];
    if (oldConnectionTarget_ == connectionGraphicsItem) oldConnectionTarget_ = NULL;
    connectionGraphicsItems_.erase(connection);
    delete connectionGraphicsItem;
}

//////////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING/DISPLAYING LINKS   //
//////////////////////////////////////////////////////////////
void NetworkEditor::removeLink(LinkConnectionGraphicsItem* linkGraphicsItem) {
    Processor* processor1 = linkGraphicsItem->getSrcProcessorGraphicsItem()->getProcessor();
    Processor* processor2 = linkGraphicsItem->getDestProcessorGraphicsItem()->getProcessor();
    removeLinkGraphicsItem(linkGraphicsItem);
    InviwoApplication::getPtr()->getProcessorNetwork()->removeLink(processor1, processor2);
}

LinkConnectionGraphicsItem* NetworkEditor::addLinkGraphicsItem(Processor* processor1,
                                                               Processor* processor2) {
    ProcessorLink* link = InviwoApplication::getPtr()
        ->getProcessorNetwork()
        ->getProcessorLink(processor1, processor2);
    ProcessorLinkGraphicsItem* p1 = getProcessorGraphicsItem(processor1)->getLinkGraphicsItem();
    ProcessorLinkGraphicsItem* p2 = getProcessorGraphicsItem(processor2)->getLinkGraphicsItem();
    
    LinkConnectionGraphicsItem* linkGraphicsItem = new LinkConnectionGraphicsItem(p1, p2, link);
    linkGraphicsItems_[link] = linkGraphicsItem;
    addItem(linkGraphicsItem);
    return linkGraphicsItem;
}

void NetworkEditor::removeLinkGraphicsItem(LinkConnectionGraphicsItem* linkGraphicsItem) {
    linkGraphicsItems_.erase(linkGraphicsItem->getProcessorLink());
    delete linkGraphicsItem;
}

void NetworkEditor::showLinkDialog(LinkConnectionGraphicsItem* linkConnectionGraphicsItem) {
    Processor* srcProcessor =
        linkConnectionGraphicsItem->getSrcProcessorGraphicsItem()->getProcessor();
    Processor* destProcessor =
        linkConnectionGraphicsItem->getDestProcessorGraphicsItem()->getProcessor();

    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    LinkDialog* linkDialog = new LinkDialog(srcProcessor, destProcessor, app->getMainWindow());

    if (!linkDialog->exec()) {
        delete linkDialog;
    }

    ProcessorLink* processorLink =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLink(srcProcessor,
                                                                             destProcessor);

    if (processorLink->getPropertyLinks().size() == 0)
        removeLink(srcProcessor, destProcessor);
}

//////////////////////////////////////
//   PORT INSPECTOR FUNCTIONALITY   //
//////////////////////////////////////

bool NetworkEditor::addPortInspector(std::string processorIdentifier, std::string portIdentifier,
                                     QPointF pos) {
    portInspectorPortIdentifier_ = portIdentifier;
    portInspectorProcessorIdentifier_ = processorIdentifier;

    Processor* processor =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorIdentifier);

    if (!processor) return false;

    Outport* port = processor->getOutport(portIdentifier);

    if (!port) return false;

    PortInspector* portInspector =
        PortInspectorFactory::getPtr()->getPortInspectorForPortClass(port->getClassIdentifier());

    if (portInspector && !portInspector->isActive()) {
        portInspector->setActive(true);
        InviwoApplication::getPtr()->getProcessorNetwork()->lock();
        // Add processors to the network
        CanvasProcessor* canvasProcessor = portInspector->getCanvasProcessor();
        std::vector<Processor*> processors = portInspector->getProcessors();

        for (size_t i = 0; i < processors.size(); i++) {
            InviwoApplication::getPtr()->getProcessorNetwork()->addProcessor(processors[i]);
        }

        addProcessorRepresentations(canvasProcessor, pos, false, false, false, false);

        // Connect the port to inspect to the inports of the inspector network
        Outport* outport = dynamic_cast<Outport*>(port);
        std::vector<Inport*> inports = portInspector->getInports();

        for (size_t i = 0; i < inports.size(); i++)
            InviwoApplication::getPtr()->getProcessorNetwork()->addConnection(outport, inports[i]);

        // Add links to the network
        std::vector<PropertyLink*> links = portInspector->getPropertyLinks();

        for (size_t i = 0; i < links.size(); i++) {
            InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                links[i]->getSourceProperty(), links[i]->getDestinationProperty());
        }

        // Do some autolinking, without adding gui stuff.
        std::vector<Processor*> existingProcessors =
            InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
        LinkSettings* linkSettings = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>();

        std::vector<Processor*> invalidProcessors;

        for (size_t h = 0; h < processors.size(); h++) {
            Processor* addedProcessor = processors[h];
            std::vector<Property*> dstProperties = addedProcessor->getProperties();

            for (size_t i = 0; i < dstProperties.size(); i++) {
                Property* dstProperty = dstProperties[i];

                for (size_t j = 0; j < existingProcessors.size(); j++) {
                    if (existingProcessors[j] != addedProcessor) {
                        std::vector<Property*> srcProperties =
                            existingProcessors[j]->getProperties();
                        bool linkCreated = false;
                        for (size_t k = 0; k < srcProperties.size(); k++) {
                            if (AutoLinker::canLink(srcProperties[k], dstProperty,
                                                    DefaultLinkingCondition) &&
                                linkSettings->isLinkable(srcProperties[k]) &&
                                linkSettings->isLinkable(dstProperty)) {
                                Property* srcProperty = srcProperties[k];
                                InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                                    srcProperty, dstProperty);
                                InviwoApplication::getPtr()->getProcessorNetwork()->addLink(
                                    dstProperty, srcProperty);
                                linkCreated = true;
                            }
                        }
                        if (linkCreated) {
                            if (std::find(invalidProcessors.begin(), invalidProcessors.end(),
                                          existingProcessors[j]) == invalidProcessors.end()) {
                                invalidProcessors.push_back(existingProcessors[j]);
                            }
                        }
                    }
                }
            }
        }

        // Setup the widget
        ProcessorWidgetQt* processorWidgetQt =
            dynamic_cast<ProcessorWidgetQt*>(canvasProcessor->getProcessorWidget());
        ivwAssert(processorWidgetQt, "Processor widget not found in inspector network.");

        int size = InviwoApplication::getPtr()
                       ->getSettingsByType<SystemSettings>()
                       ->portInspectorSize_.get();

        processorWidgetQt->setMinimumSize(size, size);
        processorWidgetQt->setMaximumSize(size, size);
        processorWidgetQt->setWindowFlags(Qt::CustomizeWindowHint | Qt::Tool );
        processorWidgetQt->move(ivec2(pos.x(), pos.y()));
        processorWidgetQt->show();
        
        // Add connections to the network
        std::vector<PortConnection*> connections = portInspector->getConnections();

        for (size_t i = 0; i < connections.size(); i++) {
            InviwoApplication::getPtr()->getProcessorNetwork()->addConnection(
                connections[i]->getOutport(), connections[i]->getInport());
        }

        for (size_t i = 0; i < invalidProcessors.size(); i++)
            InviwoApplication::getPtr()->getProcessorNetwork()->setLinkModifiedByOwner(
                invalidProcessors[i]);

        InviwoApplication::getPtr()->getProcessorNetwork()->unlock();

        if (invalidProcessors.size())
            InviwoApplication::getPtr()->getProcessorNetwork()->setLinkModifiedByOwner(
                invalidProcessors[0]);

        return true;
    }

    return false;
}

std::vector<unsigned char>* NetworkEditor::renderPortInspectorImage(Port* port, std::string type) {
    PortInspector* portInspector =
        PortInspectorFactory::getPtr()->getPortInspectorForPortClass(port->getClassIdentifier());

    ProcessorNetwork* network = InviwoApplication::getPtr()->getProcessorNetwork();
    std::vector<unsigned char>* data = NULL;

    if (portInspector && !portInspector->isActive()) {
        portInspector->setActive(true);
        network->lock();
        // Add processors to the network
        CanvasProcessor* canvasProcessor = portInspector->getCanvasProcessor();
        std::vector<Processor*> processors = portInspector->getProcessors();

        for (size_t i = 0; i < processors.size(); i++) {
            network->addProcessor(processors[i]);
        }

        addProcessorRepresentations(canvasProcessor, QPoint(0, 0), false, false, false, false);

        // Connect the port to inspect to the inports of the inspector network
        Outport* outport = dynamic_cast<Outport*>(port);
        std::vector<Inport*> inports = portInspector->getInports();

        for (size_t i = 0; i < inports.size(); i++) network->addConnection(outport, inports[i]);

        // Add links to the network
        std::vector<PropertyLink*> links = portInspector->getPropertyLinks();

        for (size_t i = 0; i < links.size(); i++) {
            network->addLink(links[i]->getSourceProperty(), links[i]->getDestinationProperty());
        }

        // Do some autolinking, without adding gui stuff.
        std::vector<Processor*> existingProcessors = network->getProcessors();
        LinkSettings* linkSettings = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>();

        std::vector<Processor*> invalidProcessors;

        for (size_t h = 0; h < processors.size(); h++) {
            Processor* addedProcessor = processors[h];
            std::vector<Property*> dstProperties = addedProcessor->getProperties();

            for (size_t i = 0; i < dstProperties.size(); i++) {
                Property* dstProperty = dstProperties[i];

                for (size_t j = 0; j < existingProcessors.size(); j++) {
                    if (existingProcessors[j] != addedProcessor) {
                        std::vector<Property*> srcProperties =
                            existingProcessors[j]->getProperties();
                        bool linkCreated = false;
                        for (size_t k = 0; k < srcProperties.size(); k++) {
                            Property* srcProperty = srcProperties[k];
                            if (AutoLinker::canLink(srcProperty, dstProperty,
                                                    DefaultLinkingCondition) &&
                                linkSettings->isLinkable(srcProperty) &&
                                linkSettings->isLinkable(dstProperty)) {
                                network->addLink(srcProperty, dstProperty);
                                linkCreated = true;
                            }
                        }
                        if (linkCreated) {
                            if (std::find(invalidProcessors.begin(), invalidProcessors.end(),
                                          existingProcessors[j]) == invalidProcessors.end()) {
                                invalidProcessors.push_back(existingProcessors[j]);
                            }
                        }
                    }
                }
            }
        }

        // Add connections to the network
        std::vector<PortConnection*> connections = portInspector->getConnections();

        for (size_t i = 0; i < connections.size(); i++) {
            network->addConnection(connections[i]->getOutport(), connections[i]->getInport());
        }

        for (size_t i = 0; i < invalidProcessors.size(); i++)
            network->setLinkModifiedByOwner(invalidProcessors[i]);


        int size = InviwoApplication::getPtr()
            ->getSettingsByType<SystemSettings>()
            ->portInspectorSize_.get();
        canvasProcessor->setCanvasSize(ivec2(size, size));

        network->unlock();
        if (invalidProcessors.size()) network->setLinkModifiedByOwner(invalidProcessors[0]);
        
        data = canvasProcessor->getImageLayerAsCodedBuffer(type);

        // remove the network...
        network->lock();
        removeProcessorRepresentations(canvasProcessor);
        // Remove processors from the network
        for (size_t i = 0; i < processors.size(); i++) network->removeProcessor(processors[i]);
        network->unlock();

        portInspector->setActive(false);
    }
   
    return data;
}

void NetworkEditor::removePortInspector() {
    std::string portIdentifier = portInspectorPortIdentifier_;
    std::string processorIdentifier = portInspectorProcessorIdentifier_;

    Processor* processor =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorByName(processorIdentifier);

    if (processor) {
        Outport* port = processor->getOutport(portIdentifier);
        if (port) {
            PortInspector* portInspector =
                PortInspectorFactory::getPtr()->getPortInspectorForPortClass(port->getClassIdentifier());

            if (portInspector && portInspector->isActive()) {
                InviwoApplication::getPtr()->getProcessorNetwork()->lock();
                CanvasProcessor* canvasProcessor = portInspector->getCanvasProcessor();
                removeProcessorRepresentations(canvasProcessor);
                // Remove processors from the network
                std::vector<Processor*> processors = portInspector->getProcessors();

                for (size_t i = 0; i < processors.size(); i++)
                    InviwoApplication::getPtr()->getProcessorNetwork()->removeProcessor(
                    processors[i]);

                InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
                portInspector->setActive(false);
            }
        }
    }

    portInspectorPortIdentifier_ = "";
    portInspectorProcessorIdentifier_ = "";
}

bool NetworkEditor::isModified() const { return modified_; }
void NetworkEditor::setModified(const bool modified) {
    if (modified != modified_) {
        modified_ = modified;
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend();
             ++it)
            static_cast<NetworkEditorObserver*>(*it)->onModifiedStatusChanged(modified);
    }
}

void NetworkEditor::cacheProcessorProperty(Processor* p) {
    std::vector<Processor*> processors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
    bool preModifiedStatus = modified_;
    if (std::find(processors.begin(), processors.end(), p) != processors.end()) {
        PropertyListWidget* propertyListWidget = PropertyListWidget::instance();
        propertyListWidget->cacheProcessorPropertiesItem(p);
    }
    setModified(preModifiedStatus);
}

////////////////////////////////////////////////////////
//   LOAD AND GET SNAPSHOT FROM EXTERNAL NETWORK      //
////////////////////////////////////////////////////////

void NetworkEditor::addExternalNetwork(std::string fileName, std::string identifierPrefix,
                                       ivec2 pos, unsigned int networkEditorFlags,
                                       ivec2 canvasSize) {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    IvwDeserializer xmlDeserializer(fileName);
    ProcessorNetwork* processorNetwork = new ProcessorNetwork();
    processorNetwork->deserialize(xmlDeserializer);
    std::vector<Processor*> processors = processorNetwork->getProcessors();

    for (size_t i = 0; i < processors.size(); i++) {
        Processor* processor = processors[i];
        std::string newIdentifier = identifierPrefix + "_" + processor->getIdentifier();
        addProcessor(processor, QPointF(pos.x, pos.y), false, false, false);
        processor->setIdentifier(newIdentifier);
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);

        if (canvasProcessor) {
            // show processor widget as tool window
            ProcessorWidgetQt* processorWidgetQt =
                dynamic_cast<ProcessorWidgetQt*>(processor->getProcessorWidget());
            ivwAssert(processorWidgetQt, "Processor widget not found in inspector network.");

            if (!(networkEditorFlags & NetworkEditor::UseOriginalCanvasSize)) {
                processorWidgetQt->setMinimumSize(canvasSize[0], canvasSize[1]);
                processorWidgetQt->setMaximumSize(canvasSize[0], canvasSize[1]);
            }

            processorWidgetQt->move(pos);
            processorWidgetQt->show();
            InviwoApplication::getPtr()->getProcessorNetworkEvaluator()->registerCanvas(
                canvasProcessor->getCanvas(),
                canvasProcessor
                    ->getIdentifier());  // register is required because identifier is modified.
        }
    }

    std::vector<PortConnection*> connections = processorNetwork->getConnections();

    for (size_t i = 0; i < connections.size(); i++) {
        PortConnection* connection = connections[i];
        Outport* outport = connection->getOutport();
        Inport* inport = connection->getInport();
        // first remove the connection from the loaded network to avoid an already connected warning
        processorNetwork->removeConnection(outport, inport);
        addConnection(outport, inport);
    }

    std::vector<ProcessorLink*> links = processorNetwork->getProcessorLinks();

    for (size_t i = 0; i < links.size(); i++) {
        addLink(links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        ProcessorLink* link = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLink(
            links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        std::vector<PropertyLink*> propertyLinks = links[i]->getPropertyLinks();

        for (size_t j = 0; j < propertyLinks.size(); j++) link->addPropertyLinks(propertyLinks[j]);
    }

    processors = processorNetwork->getProcessors();

    for (size_t i = 0; i < processors.size(); i++) {
        Processor* processor = processors[i];

        if (processor->hasProcessorWidget()) processor->getProcessorWidget()->show();
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
}

std::vector<std::string> NetworkEditor::saveSnapshotsInExternalNetwork(
    std::string externalNetworkFile, std::string identifierPrefix) {
    // turnoff sound
    BoolProperty* soundProperty = dynamic_cast<BoolProperty*>(
        InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getPropertyByIdentifier(
            "enableSound"));
    bool isSoundEnabled = soundProperty->get();

    if (isSoundEnabled) soundProperty->set(false);

    std::vector<std::string> canvasSnapShotFiles;
    std::string directory = URLParser::getFileDirectory(externalNetworkFile);
    std::string workSpaceName = URLParser::getFileNameWithExtension(externalNetworkFile);
    std::string newFileName = URLParser::replaceFileExtension(workSpaceName, "png");
    std::vector<Processor*> processors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

    for (size_t i = 0; i < processors.size(); i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix) != std::string::npos) {
            CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processors[i]);

            if (canvasProcessor) {
                std::string snapShotFilePath =
                    directory + "snapshot_" + canvasProcessor->getIdentifier() + newFileName;
                canvasSnapShotFiles.push_back(snapShotFilePath);
                canvasProcessor->saveImageLayer(snapShotFilePath.c_str());
            }
        }
    }

    if (isSoundEnabled) soundProperty->set(true);

    return canvasSnapShotFiles;
}

void NetworkEditor::removeExternalNetwork(std::string identifierPrefix) {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();
    std::vector<Processor*> processors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

    for (size_t i = 0; i < processors.size(); i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix) != std::string::npos)
            removeProcessor(processors[i]);
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
}

std::vector<std::string> NetworkEditor::getSnapshotsOfExternalNetwork(std::string fileName) {
    std::vector<std::string> snapshotFileNames;
    // load external network
    QRectF rect = sceneRect();
    ivec2 pos(rect.width() / 2, rect.height() / 2);
    std::string identifierPrefix = "TemporaryExternalNetwork";
    unsigned int networkEditorFlags =
        NetworkEditor::UseOriginalCanvasSize | NetworkEditor::CanvasHidden;
    addExternalNetwork(fileName, identifierPrefix, pos, networkEditorFlags);
    InviwoApplication::getPtr()->getProcessorNetwork()->setModified(true);
    InviwoApplication::getPtr()->getProcessorNetworkEvaluator()->requestEvaluate();
    // save snapshot
    snapshotFileNames = saveSnapshotsInExternalNetwork(fileName, identifierPrefix);
    // unload external network
    removeExternalNetwork(identifierPrefix);
    return snapshotFileNames;
}

////////////////////////////////////////////
//   OBTAIN GRAPHICS ITEMS FROM NETWORK   //
////////////////////////////////////////////
ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItem(Processor* key) const {
    ProcessorMap::const_iterator it = processorGraphicsItems_.find(key);

    if (it != processorGraphicsItems_.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItem(PortConnection* key) const {
    ConnectionMap::const_iterator it = connectionGraphicsItems_.find(key);

    if (it != connectionGraphicsItems_.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItem(ProcessorLink* key) const {
    LinkMap::const_iterator it = linkGraphicsItems_.find(key);

    if (it != linkGraphicsItems_.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItem(Processor* processor1,
                                                               Processor* processor2) const {
    ProcessorLink* link = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLink(
        processor1, processor2);

    return getLinkGraphicsItem(link);
}

ProcessorGraphicsItem* NetworkEditor::getProcessorGraphicsItemAt(const QPointF pos) const {
    return getGraphicsItemAt<ProcessorGraphicsItem>(pos);
}
ProcessorInportGraphicsItem* NetworkEditor::getProcessorInportGraphicsItemAt(
    const QPointF pos) const {
    return getGraphicsItemAt<ProcessorInportGraphicsItem>(pos);
}
ConnectionGraphicsItem* NetworkEditor::getConnectionGraphicsItemAt(const QPointF pos) const {
    return getGraphicsItemAt<ConnectionGraphicsItem>(pos);
}
LinkConnectionGraphicsItem* NetworkEditor::getLinkGraphicsItemAt(const QPointF pos) const {
    return getGraphicsItemAt<LinkConnectionGraphicsItem>(pos);
}

////////////////////////////////
//   EVENT HANDLING METHODS   //
////////////////////////////////
void NetworkEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsScene::mousePressEvent(e);
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // Connection drag mode
        connectionCurve_->setEndPoint(e->scenePos());
        connectionCurve_->reactToPortHover(getProcessorInportGraphicsItemAt(e->scenePos()));
        e->accept();

    } else if (linkCurve_) {
        // Link drag mode
        linkCurve_->setEndPoint(e->scenePos());
        linkCurve_->reactToProcessorHover(getProcessorGraphicsItemAt(e->scenePos()));
        e->accept();
    }

    QGraphicsScene::mouseMoveEvent(e);
}

void NetworkEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // connection drag mode
        Outport* startPort = connectionCurve_->getOutportGraphicsItem()->getPort();

        delete connectionCurve_;
        connectionCurve_ = NULL;

        ProcessorInportGraphicsItem* endItem = getProcessorInportGraphicsItemAt(e->scenePos());
        if (endItem && endItem->getPort()->canConnectTo(startPort)) {
            Inport* endPort = endItem->getPort();

            if (endPort->isConnected()) {
                SingleInport* singleInport = dynamic_cast<SingleInport*>(endPort);
                if (singleInport) {
                    removeConnection(singleInport->getConnectedOutport(), endPort);
                }
            }
            addConnection(startPort, endPort);
        }
        e->accept();

    } else if (linkCurve_) {
        // link drag mode
        ProcessorGraphicsItem* endProcessorItem = getProcessorGraphicsItemAt(e->scenePos());
        if (endProcessorItem) {
            Processor* endProcessor = endProcessorItem->getProcessor();
            Processor* startProcessor = linkCurve_->getSrcProcessorLinkGraphicsItem()
                                            ->getProcessorGraphicsItem()
                                            ->getProcessor();

            if (startProcessor != endProcessor) {
                LinkConnectionGraphicsItem* linkGraphicsItem =
                    addLink(startProcessor, endProcessor);
                showLinkDialog(linkGraphicsItem);
            }
        }

        delete linkCurve_;
        linkCurve_ = NULL;
        e->accept();
    }

    QGraphicsScene::mouseReleaseEvent(e);
}

void NetworkEditor::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkConnectionGraphicsItem* linkGraphicsItem = getLinkGraphicsItemAt(e->scenePos());
    ProcessorGraphicsItem* processorGraphicsItem = getProcessorGraphicsItemAt(e->scenePos());
    if (processorGraphicsItem) {
        processorGraphicsItem->editProcessorName();
        e->accept();
    } else if (linkGraphicsItem) {
        showLinkDialog(linkGraphicsItem);
        e->accept();
    } else {
        QGraphicsScene::mouseDoubleClickEvent(e);
    }
}

void NetworkEditor::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Delete) {
        e->accept();
        // delete selected graphics items
        // check if selected is connection and delete it
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();

        for (int i = 0; i < selectedGraphicsItems.size(); i++) {
            ConnectionGraphicsItem* connectionGraphicsItem =
                qgraphicsitem_cast<ConnectionGraphicsItem*>(selectedGraphicsItems[i]);

            if (connectionGraphicsItem) removeConnection(connectionGraphicsItem);
        }

        // check if selected is link and delete it
        selectedGraphicsItems = selectedItems();

        for (int i = 0; i < selectedGraphicsItems.size(); i++) {
            LinkConnectionGraphicsItem* linkGraphicsItem =
                qgraphicsitem_cast<LinkConnectionGraphicsItem*>(selectedGraphicsItems[i]);

            if (linkGraphicsItem) removeLink(linkGraphicsItem);
        }

        // check if selected is processor and delete it
        selectedGraphicsItems = selectedItems();

        for (int i = 0; i < selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem =
                qgraphicsitem_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);

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
            ProcessorWidget* processorWidget =
                processorGraphicsItem->getProcessor()->getProcessorWidget();

            if (processorWidget && processorWidget->isVisible())
                showAction->setChecked(true);
            else
                showAction->setChecked(false);
        }
        QAction* result = menu.exec(QCursor::pos());

        if (result == renameAction) {
            clearSelection();
            processorGraphicsItem->setSelected(true);
            processorGraphicsItem->editProcessorName();
        
        } else if (result == deleteAction) {
            Processor* processor = processorGraphicsItem->getProcessor();
            removeProcessor(processor);

        } else if (showAction && result == showAction) {
            if (showAction->isChecked()) {
                processorGraphicsItem->getProcessor()->getProcessorWidget()->show();
            } else {
                processorGraphicsItem->getProcessor()->getProcessorWidget()->hide();
            }
        }

    } else if (connectionGraphicsItem) {
        QMenu menu;
        QAction* action = menu.addAction(tr("Delete"));
        QAction* result = menu.exec(QCursor::pos());
        if (result == action) removeConnection(connectionGraphicsItem);

    } else if (linkGraphicsItem) {
        QMenu menu;
        QAction* deleteAction = menu.addAction("Delete");
        QAction* result = menu.exec(QCursor::pos());
        if (result == deleteAction) removeLink(linkGraphicsItem);

    } else {
        QGraphicsScene::contextMenuEvent(e);
    }
}

/////////////////////////////////////////
//   PROCESSOR DRAG AND DROP METHODS   //
/////////////////////////////////////////
void NetworkEditor::dragEnterEvent(QGraphicsSceneDragDropEvent* e) { dragMoveEvent(e); }

void NetworkEditor::dragMoveEvent(QGraphicsSceneDragDropEvent* e) {
    if (ProcessorDragObject::canDecode(e->mimeData())) {
        e->setAccepted(true);
        e->acceptProposedAction();
        ConnectionGraphicsItem* connectionItem = getConnectionGraphicsItemAt(e->scenePos());

        if (connectionItem && !oldConnectionTarget_) {  //< New connection found
            QString className;
            ProcessorDragObject::decode(e->mimeData(), className);
            Processor* processor = static_cast<Processor*>(
                ProcessorFactory::getPtr()->create(className.toLocal8Bit().constData()));
            
            bool inputmatch = false;
            // try to find a match between connection outport and one of the new processors inports
            for (size_t i = 0; i < processor->getInports().size(); ++i) {
                if (processor->getInports().at(i)->canConnectTo(connectionItem->getOutport())) {
                    inputmatch = true;
                    break;
                }
            }

            bool outputmatch = false;
            // try to find a match between connection inport and one of the new processors outports
            for (size_t i = 0; i < processor->getOutports().size(); ++i) {
                if (connectionItem->getInport()->canConnectTo(processor->getOutports().at(i))) {
                    outputmatch = true;
                    break;
                }
            }
            if(inputmatch && outputmatch) {
                connectionItem->setBorderColor(Qt::green);
            } else {
                connectionItem->setBorderColor(Qt::red);
            }
            oldConnectionTarget_ = connectionItem;
            oldConnectionTarget_->setMidPoint(e->scenePos());
            delete processor;

        } else if (connectionItem) {  // move event on active connection
            oldConnectionTarget_->setMidPoint(e->scenePos());

        } else if (oldConnectionTarget_ && !connectionItem) {  //< Connection no longer targeted
            oldConnectionTarget_->resetBorderColors();
            oldConnectionTarget_->clearMidPoint();
            oldConnectionTarget_ = NULL;

        } else if (!connectionItem) {  // processor replacement
            ProcessorGraphicsItem* processorItem = getProcessorGraphicsItemAt(e->scenePos());

            if (processorItem && !oldProcessorTarget_) {  //< New processor found
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
            e->setAccepted(true);
            e->acceptProposedAction();

            // create processor, add it to processor network, and generate it's widgets
            Processor* processor = static_cast<Processor*>(
                ProcessorFactory::getPtr()->create(className.toLocal8Bit().constData()));

            clearSelection();

            ProcessorGraphicsItem* processorGraphicsItem;
            if (oldProcessorTarget_) {
               processorGraphicsItem = addProcessor(processor, oldProcessorTarget_->scenePos());
            } else {
               processorGraphicsItem = addProcessor(processor, e->scenePos());
            }

            // check for collisions
            if (oldConnectionTarget_) {
                placeProcessorOnConnection(processorGraphicsItem, oldConnectionTarget_);
            } else if (oldProcessorTarget_) {
                placeProcessorOnProcessor(processorGraphicsItem, oldProcessorTarget_);
            }

            // clear oldDragTarget
            oldConnectionTarget_ = NULL;
            oldProcessorTarget_ = NULL;
        }
    }
}

void NetworkEditor::placeProcessorOnConnection(ProcessorGraphicsItem* processorItem,
                                               ConnectionGraphicsItem* connectionItem) {
    std::vector<Inport*> inports = processorItem->getProcessor()->getInports();
    std::vector<Outport*> outports = processorItem->getProcessor()->getOutports();
    Inport* connectionInport = connectionItem->getInport();
    Outport* connectionOutport = connectionItem->getOutport();

    InviwoApplication::getPtr()->getProcessorNetwork()->lock();


    Inport* inport = NULL;
    Outport* outport = NULL;

    for (size_t i = 0; i < inports.size(); ++i) {
        if (inports.at(i)->canConnectTo(connectionOutport)) {
            inport = inports.at(i);
            break;
        }
    }
    for (size_t i = 0; i < outports.size(); ++i) {
        if (connectionInport->canConnectTo(outports.at(i))) {
            outport = outports.at(i);
            break;
        }
    }

    if(inport && outport) {
        // remove old connection
        removeConnection(connectionItem);

        addConnection(connectionOutport, inport); 
        addConnection(outport, connectionInport);
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
}

void NetworkEditor::placeProcessorOnProcessor(ProcessorGraphicsItem* processorItem,
                                              ProcessorGraphicsItem* oldProcessorItem) {

    ProcessorNetwork* network = InviwoApplication::getPtr()->getProcessorNetwork();
    Processor* newProcessor = processorItem->getProcessor();
    Processor* oldProcessor = oldProcessorItem->getProcessor();

    const std::vector<Inport*>& inports = newProcessor->getInports();
    const std::vector<Outport*>& outports = newProcessor->getOutports();
    const std::vector<Inport*>& oldInports = oldProcessor->getInports();
    const std::vector<Outport*>& oldOutports = oldProcessor->getOutports();

    network->lock();

    std::vector<std::pair<Outport*, Inport*> > newConnections;

    for (size_t i = 0; i < std::min(inports.size(), oldInports.size()); ++i) {
        if (inports.at(i)->canConnectTo(oldInports.at(i)->getConnectedOutport())) {
            // MultiInports may have several connected outports
            std::vector<Outport*> previouslyConnectedOutports =
                oldInports.at(i)->getConnectedOutports();
            for (std::vector<Outport*>::iterator it = previouslyConnectedOutports.begin(),
                                                 itEnd = previouslyConnectedOutports.end();
                 it != itEnd; ++it) {
                // save new connection connectionOutportoldInport-processorInport
                newConnections.push_back(std::make_pair(*it, inports.at(i)));
            }
        }
    }

    for (size_t i = 0; i < std::min(outports.size(), oldOutports.size()); ++i) {
        std::vector<Inport*> previouslyConnectedInports = oldOutports.at(i)->getConnectedInports();

        for (size_t j = 0; j < previouslyConnectedInports.size(); ++j) {
            if (previouslyConnectedInports.at(j)->canConnectTo(outports.at(i))) {
                // save new connection processorOutport-connectionInport
                newConnections.push_back(
                    std::make_pair(outports.at(i), previouslyConnectedInports.at(j)));
            }
        }
    }
    
    
    // Copy over the value of old props to new ones if id and classname are equal.
    std::vector<Property*> newProps = newProcessor->getProperties();
    std::vector<Property*> oldProps = oldProcessor->getProperties();
    
    std::map<Property*, Property*> propertymap;
    
    for (std::vector<Property*>::iterator newit = newProps.begin(); newit != newProps.end(); ++newit) {
        for (std::vector<Property*>::iterator oldit = oldProps.begin(); oldit != oldProps.end(); ++oldit) {
            if ( (*newit)->getIdentifier() == (*oldit)->getIdentifier()
                && (*newit)->getClassIdentifier() == (*oldit)->getClassIdentifier()) {
                (*newit)->set(*oldit);
                propertymap[(*oldit)] = (*newit);
            }
        }
    }
    
    // Move propertylinks to the new processor
    std::vector<PropertyLink*> links = network->getLinks();
    std::map<Property*, Property*>::iterator match;
    
    for (std::vector<Property*>::iterator oldit = oldProps.begin(); oldit != oldProps.end(); ++oldit) {
        for (std::vector<PropertyLink*>::iterator linkit = links.begin(); linkit != links.end(); ++linkit) {
            if ( (*linkit)->getDestinationProperty() == (*oldit) ) {
                match = propertymap.find(*oldit);
                if( match != propertymap.end()) {
                    // add link from
                    Property* start = (*linkit)->getSourceProperty();
                    // to
                    Property* end = match->second;
                    
                    addLink(start->getOwner(), end->getOwner());
                    network->addLink(start, end);
                }
            }
            if ( (*linkit)->getSourceProperty() == (*oldit) ) {
                match = propertymap.find(*oldit);
                if( match != propertymap.end()) {
                    // add link from
                    Property* start = match->second;
                     //to
                    Property* end = (*linkit)->getDestinationProperty();
                    
                    addLink(start->getOwner(), end->getOwner());
                    network->addLink(start, end);
                }
            }
        }
    }
       
    // remove old processor
    removeProcessor(oldProcessorItem->getProcessor());

    // create all new connections
    for (size_t i = 0; i < newConnections.size(); ++i)
        addConnection(newConnections.at(i).first, newConnections.at(i).second);

    network->unlock();
}

///////////////////////////////
//   SERIALIZATION METHODS   //
///////////////////////////////
void NetworkEditor::clearNetwork() {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();

    // Invalidate inports to alert processors that they should stop their calculations.
    std::vector<Processor*> processors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();
    for (size_t p = 0; p < processors.size(); p++) {
        std::vector<Inport*> inports = processors[p]->getInports();
        for (size_t i = 0; i < inports.size(); i++)
            inports[i]->invalidate(PropertyOwner::INVALID_OUTPUT);
    }

    ResourceManager::getPtr()->clearAllResources();

    removePortInspector();

    for (size_t i = 0; i < processors.size(); i++) removeProcessor(processors[i]);

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
    setModified(true);
}

bool NetworkEditor::saveNetwork(std::string fileName) {
    removePortInspector();
    try {
        IvwSerializer xmlSerializer(fileName);
        InviwoApplication::getPtr()->getProcessorNetwork()->serialize(xmlSerializer);
        InviwoApplication::getPtr()->getProcessorNetwork()->setModified(false);
        xmlSerializer.writeFile();
        filename_ = fileName;
        setModified(false);
    } catch (SerializationException& exception) {
        LogInfo("Unable to save network " + fileName + " due to " + exception.getMessage());
        return false;
    }
    return true;
}

bool NetworkEditor::saveNetwork(std::ostream stream) {
    removePortInspector();
    try {
        IvwSerializer xmlSerializer(filename_);
        InviwoApplication::getPtr()->getProcessorNetwork()->serialize(xmlSerializer);
        InviwoApplication::getPtr()->getProcessorNetwork()->setModified(false);
        xmlSerializer.writeFile(stream);
        setModified(false);
    } catch (SerializationException& exception) {
        LogInfo("Unable to save network " + filename_ + " due to " + exception.getMessage());
        return false;
    }
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {
    std::ifstream fileStream(fileName.c_str());
    if (!fileStream) {
        LogError("Could not open workspace file: " << fileName);
        fileStream.close();
        return false;
    }
    bool loaded = loadNetwork(fileStream, fileName);
    fileStream.close();
    if (loaded) {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend();
             ++it)
            static_cast<NetworkEditorObserver*>(*it)->onNetworkEditorFileChanged(fileName);
    }

    return loaded;
}

bool NetworkEditor::loadNetwork(std::istream& stream, const std::string& path) {
    // first we clean the current network
    clearNetwork();
    // then we lock the network that no evaluations are triggered during the deserialization
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();

    // then we deserialize processor network
    try {
        IvwDeserializer xmlDeserializer(stream, path);
        InviwoApplication::getPtr()->getProcessorNetwork()->deserialize(xmlDeserializer);
    } catch (const AbortException& exception) {
        LogInfo("Unable to load network " + path + " due to " + exception.getMessage());
        clearNetwork();
        InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
        return false;
    } catch (const IgnoreException& exception) {
        LogInfo("Incomplete network loading " + path + " due to " + exception.getMessage());
    }

    // add processors
    std::vector<Processor*> processors =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

    for (size_t i = 0; i < processors.size(); i++) {
        processors[i]->invalidate(PropertyOwner::INVALID_RESOURCES);
        ProcessorMetaData* meta =
            dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));
        addProcessorRepresentations(processors[i],
                                    QPointF(meta->getPosition().x, meta->getPosition().y),
                                    meta->isVisible(), meta->isSelected(), false, false);
    }
    PropertyListWidget::instance()->setUsageMode(InviwoApplication::getPtr()
                                                     ->getSettingsByType<SystemSettings>()
                                                     ->applicationUsageModeProperty_.get());

    // add connections
    std::vector<PortConnection*> connections =
        InviwoApplication::getPtr()->getProcessorNetwork()->getConnections();

    for (size_t i = 0; i < connections.size(); i++) addConnectionGraphicsItem(connections[i]);

    // add link graphics items
    std::vector<ProcessorLink*> links =
        InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLinks();

    for (size_t i = 0; i < links.size(); i++) {
        Processor* src = dynamic_cast<Processor*>(links[i]->getSourceProcessor());
        Processor* dst = dynamic_cast<Processor*>(links[i]->getDestinationProcessor());
        addLinkGraphicsItem(src, dst);
    }

    // flag the network's modified flag
    InviwoApplication::getPtr()->getProcessorNetwork()->setModified(true);
    // show all processor widgets that where hidden on network load
    bool evaluate = false;

    for (size_t i = 0; i < processors.size(); i++) {
        if (processors[i]->hasProcessorWidget()) {
            ProcessorMetaData* meta =
                dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));

            if (meta->isVisible()) {
                processors[i]->getProcessorWidget()->show();
                evaluate = true;
            }
        }
    }

    // unlock it and initiate evaluation
    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();

    // Queue event to cache all the widgets since the creation can be slow.
    for (std::vector<Processor*>::iterator it = processors.begin(); it != processors.end(); ++it) {
        QCoreApplication::postEvent(
            PropertyListWidget::instance(),
            new PropertyListEvent(PropertyListEvent::CACHE, (*it)->getIdentifier()),
            Qt::LowEventPriority);
    }

    QCoreApplication::postEvent(this, new QEvent(QEvent::Type(cacheProcessorPropertyDoneEventId_)),
                                Qt::LowEventPriority);

    setModified(false);
    filename_ = path;
    return true;
}

bool NetworkEditor::event(QEvent* e) {
    if (e->type() == cacheProcessorPropertyDoneEventId_) {
        QCoreApplication::postEvent(this, new QEvent(QEvent::Type(markModifedFlaseEventId_)),
                                    Qt::LowEventPriority);
        e->accept();
        return true;
    } else if (e->type() == markModifedFlaseEventId_) {
        e->accept();
        setModified(false);
        return true;
    }
    return QGraphicsScene::event(e);
}

////////////////////////
//   HELPER METHODS   //
////////////////////////
QPointF NetworkEditor::snapToGrid(QPointF pos) {
    QPointF result;

    float ox = pos.x() > 0 ? 0.5 : -0.5;
    float oy = pos.y() > 0 ? 0.5 : -0.5;

    result.setX((int(pos.x() / GRID_SPACING + ox)) * GRID_SPACING);
    result.setY((int(pos.y() / GRID_SPACING + oy)) * GRID_SPACING);
    return result;
}

void NetworkEditor::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->save();
    painter->setWorldMatrixEnabled(true);
    painter->fillRect(rect, Qt::darkGray);
    qreal left = int(rect.left()) - (int(rect.left()) % GRID_SPACING);
    qreal top = int(rect.top()) - (int(rect.top()) % GRID_SPACING);
    QVarLengthArray<QLineF, 100> linesX;
    painter->setPen(QColor(153,153,153));

    for (qreal x = left; x < rect.right(); x += GRID_SPACING)
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;

    for (qreal y = top; y < rect.bottom(); y += GRID_SPACING)
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
    painter->restore();

    // For testing purpuses only. Draw bounding rects around all graphics items
    // QList<QGraphicsItem*> items = QGraphicsScene::items(Qt::DescendingOrder);
    // painter->setPen(Qt::magenta);
    // for (QList<QGraphicsItem*>::iterator it = items.begin(); it != items.end(); ++it) {
    //    QRectF br = (*it)->sceneBoundingRect();
    //    painter->drawRect(br);
    //}
    // painter->setPen(Qt::red);
    // painter->drawRect(QGraphicsScene::itemsBoundingRect());
}

void NetworkEditor::initiateConnection(ProcessorOutportGraphicsItem* item) {
    QPointF pos = item->mapToScene(item->rect().center());
    connectionCurve_ = new ConnectionDragGraphicsItem(item, pos, item->getPort()->getColorCode());
    addItem(connectionCurve_);
    connectionCurve_->show();
}

void NetworkEditor::releaseConnection(ProcessorInportGraphicsItem* item) {
    // remove the old connection and add a new connection curve to be connected.
    ConnectionGraphicsItem* oldConnection = item->getConnections()[0];
    connectionCurve_ = new ConnectionDragGraphicsItem(oldConnection->getOutportGraphicsItem(),
                                                      oldConnection->getEndPoint(),
                                                      oldConnection->getOutport()->getColorCode());
    removeConnection(oldConnection);
    addItem(connectionCurve_);
    connectionCurve_->show();
}

void NetworkEditor::initiateLink(ProcessorLinkGraphicsItem* item, QPointF pos) {
    linkCurve_ = new LinkConnectionDragGraphicsItem(item, pos);
    addItem(linkCurve_);
    linkCurve_->setZValue(DRAGING_ITEM_DEPTH);
    linkCurve_->show();
}

void NetworkEditor::updateLeds() {
    // Update the status items
    for (ProcessorMap::iterator it = processorGraphicsItems_.begin();
         it != processorGraphicsItems_.end(); it++) {
        it->second->getStatusItem()->update();
    }
}

// Manage various tooltips.
void NetworkEditor::helpEvent(QGraphicsSceneHelpEvent* e) {
    QList<QGraphicsItem*> graphicsItems = items(e->scenePos());
    removePortInspector();
    for (int i = 0; i < graphicsItems.size(); ++i) {
        QGraphicsItem* item = graphicsItems[i];
        switch (item->type()) {
            case ProcessorGraphicsItem::Type:
                qgraphicsitem_cast<ProcessorGraphicsItem*>(item)->showToolTip(e);
                return;
            case ConnectionGraphicsItem::Type:
                qgraphicsitem_cast<ConnectionGraphicsItem*>(item)->showToolTip(e);
                return;
            case LinkConnectionGraphicsItem::Type:
                qgraphicsitem_cast<LinkConnectionGraphicsItem*>(item)->showToolTip(e);
                return;

            //case ProcessorLinkGraphicsItem::Type:
            //    qgraphicsitem_cast<ProcessorLinkGraphicsItem*>(item)->showToolTip(e);
            //    return;
            case ProcessorInportGraphicsItem::Type:
                qgraphicsitem_cast<ProcessorInportGraphicsItem*>(item)->showToolTip(e);
                return;
            case ProcessorOutportGraphicsItem::Type:
                qgraphicsitem_cast<ProcessorOutportGraphicsItem*>(item)->showToolTip(e);
                return;
            //case ProcessorStatusGraphicsItem::Type:
            //    qgraphicsitem_cast<ProcessorStatusGraphicsItem*>(item)->showToolTip(e);
            //    return;
            //case ProcessorProgressGraphicsItem::Type:
            //    qgraphicsitem_cast<ProcessorProgressGraphicsItem*>(item)->showToolTip(e);
            //    return;
        };
    }
    QGraphicsScene::helpEvent(e);
}

}  // namespace
