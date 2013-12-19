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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/multidatainport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/resources/resourcemanager.h>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QVarLengthArray>
#include <QGraphicsItem>

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

	oldConnectionTarget_ = NULL;
	oldProcessorTarget_ = NULL;
    workerThreadReset();

    processorNetwork_ = new ProcessorNetwork();
    InviwoApplication::getRef().setProcessorNetwork(processorNetwork_);
    processorNetworkEvaluator_ = new ProcessorNetworkEvaluator(processorNetwork_);

    hoverTimer_.setSingleShot(true);
    connect(&hoverTimer_, SIGNAL(timeout()), this, SLOT(hoverPortTimeOut()));
}

NetworkEditor::~NetworkEditor(){
    workerThreadQuit();
}

/////////////////////////////////////////////
//   PUBLIC METHODS FOR CHANGING NETWORK   //
/////////////////////////////////////////////
void NetworkEditor::addProcessor(Processor* processor, QPointF pos, bool showProcessor, bool showPropertyWidgets, bool showProcessorWidget) {
    // add the processor to the network    
    processor->setIdentifier(obtainUniqueProcessorID(processor->getClassName()));
    processor->initialize();
    processorNetwork_->addProcessor(processor);
    // add processor representations
    addProcessorRepresentations(processor, pos, showProcessor, showPropertyWidgets, showProcessorWidget);
}

void NetworkEditor::removeProcessor(Processor* processor) {
    // remove processor representations
    removeProcessorRepresentations(processor);
    // remove the processor from the network
    processorNetwork_->removeProcessor(processor);
    // enable default rendering context before deinitializing processor
    processorNetworkEvaluator_->activateDefaultRenderContext();
    processor->deinitialize();
    delete processor;
}


void NetworkEditor::addConnection(Outport* outport, Inport* inport) {
    processorNetwork_->addConnection(outport, inport);
    addConnectionGraphicsItem(outport, inport);
    CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(inport->getProcessor());
    ImageInport* imageInport = dynamic_cast<ImageInport*>(inport);
    ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(outport);
    if (canvasProcessor && imageInport) {                    
        ResizeEvent resizeEvent(canvasProcessor->getCanvas()->size());
        imageInport->changeDataDimensions(&resizeEvent);
    }
    else if (!canvasProcessor && imageInport) {
        ResizeEvent resizeEvent(imageOutport->getDimensions());
        imageInport->changeDataDimensions(&resizeEvent);
    }
}

void NetworkEditor::removeConnection(Outport* outport, Inport* inport) {
    removeConnectionGraphicsItem(getConnectionGraphicsItem(outport, inport));
    processorNetwork_->removeConnection(outport, inport);	
}


void NetworkEditor::addLink(Processor* processor1, Processor* processor2) {
    if (!processorNetwork_->getProcessorLink(processor1, processor2)) {
        processorNetwork_->addLink(processor1, processor2);
        addLinkGraphicsItem(processor1, processor2);
    }	
}

void NetworkEditor::removeLink(Processor* processor1, Processor* processor2) {
    removeLinkGraphicsItem(getLinkGraphicsItem(processor1, processor2));
    processorNetwork_->removeLink(processor1, processor2);	
}



////////////////////////////////////////////////////////
//   PRIVATE METHODS FOR ADDING/REMOVING PROCESSORS   //
////////////////////////////////////////////////////////
void NetworkEditor::addProcessorRepresentations(Processor* processor, QPointF pos, bool showProcessor, bool showPropertyWidgets, bool showProcessorWidget) {
    // generate GUI representations (graphics item, property widget, processor widget)
    addProcessorGraphicsItem(processor, pos, showProcessor);
    addPropertyWidgets(processor, showPropertyWidgets);
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
    removePropertyWidgets(processor);
    // processor widget already removed when processor is destroyed
}

void NetworkEditor::addProcessorGraphicsItem(Processor* processor, QPointF pos, bool visible) {
    // generate GUI representation and add to editor
    ProcessorGraphicsItem* processorGraphicsItem = new ProcessorGraphicsItem();
    processorGraphicsItem->setProcessor(processor);
    processorGraphicsItem->setVisible(visible);

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
            connectionGraphicsItems[i]->getInProcessor() == processorGraphicsItem) {
            removeConnectionGraphicsItem(connectionGraphicsItems[i]);
        }
    }

    // remove link graphics items
    std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems = linkGraphicsItems_;
    for (size_t i=0; i<linkGraphicsItems.size(); i++) {
        if (linkGraphicsItems[i]->getOutProcessorGraphicsItem() == processorGraphicsItem ||
            linkGraphicsItems[i]->getInProcessorGraphicsItem() == processorGraphicsItem) {
            removeLinkGraphicsItem(linkGraphicsItems[i]);
        }
    }

    // remove processor graphics item from editor
    processorGraphicsItem->hide();
    processorGraphicsItems_.erase(std::remove(processorGraphicsItems_.begin(), processorGraphicsItems_.end(),
                                              processorGraphicsItem), processorGraphicsItems_.end());

    // delete processor graphics item
    delete processorGraphicsItem;
}

void NetworkEditor::addPropertyWidgets(Processor* processor, bool visible) {
    // FIXME: show property widgets, but then also deselect all other processors and select this one
    if (visible){ 
        PropertyListWidget::instance()->showProcessorProperties(processor); 
    }
}

void NetworkEditor::removePropertyWidgets(Processor* processor) {
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    propertyListWidget_->removeProcessorProperties(processor);
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
    Processor* processor1 = linkGraphicsItem->getOutProcessorGraphicsItem()->getProcessor();
    Processor* processor2 = linkGraphicsItem->getInProcessorGraphicsItem()->getProcessor();
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
    Processor* inProcessor = linkConnectionGraphicsItem->getInProcessorGraphicsItem()->getProcessor();
    Processor* outProcessor = linkConnectionGraphicsItem->getOutProcessorGraphicsItem()->getProcessor();

    LinkDialog* linkDialog = new LinkDialog(inProcessor, outProcessor, processorNetwork_, 0);
    linkDialog->exec();

    ProcessorLink* processorLink = processorNetwork_->getProcessorLink(inProcessor, outProcessor);
    if (!processorLink->getPropertyLinks().size())
        removeLink(inProcessor, outProcessor);
}

void  NetworkEditor::updateLinkGraphicsItems() {
    if( isLinkDisplayEnabled() ) {
        for (size_t i=0;i<linkGraphicsItems_.size(); i++) {
            linkGraphicsItems_[i]->setVisible(true);
            ProcessorGraphicsItem* processorGraphicsItem1 = linkGraphicsItems_[i]->getOutProcessorGraphicsItem();
            ProcessorGraphicsItem* processorGraphicsItem2 = linkGraphicsItems_[i]->getInProcessorGraphicsItem();
            linkGraphicsItems_[i]->setVisible(processorGraphicsItem1->isVisible() && processorGraphicsItem2->isVisible());
        }
    }
    else {
        for (size_t i=0;i<linkGraphicsItems_.size(); i++) {
            linkGraphicsItems_[i]->setVisible(false);            
        }
    }        
}


//////////////////////////////////////
//   PORT INSPECTOR FUNCTIONALITY   //
//////////////////////////////////////
void NetworkEditor::addInspectorNetwork(Port* port, ivec2 pos, std::string inspectorNetworkFileName) {

    std::string identifierPrefix = port->getProcessor()->getIdentifier()+":"+port->getIdentifier();   

    processorNetwork_->lock();
    addExternalNetwork(inspectorNetworkFileName, identifierPrefix, pos);
    processorNetwork_->setBroadcastModification(false);
    
    //connect input port to appropriate port in inspector network
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix)!=std::string::npos) {
            Processor* processor = processors[i];
            std::vector<Inport*> inports = processor->getInports();
            for (size_t i=0; i<inports.size(); i++) {
                if (!inports[i]->isConnected()) {
                    Outport* outport = dynamic_cast<Outport*>(port);
                    if (dynamic_cast<ImageOutport*>(outport) && dynamic_cast<ImageInport*>(inports[i]))
                        processorNetwork_->addConnection(outport, inports[i]);
                    else if (dynamic_cast<VolumeOutport*>(outport) && dynamic_cast<VolumeInport*>(inports[i]))
                            processorNetwork_->addConnection(outport, inports[i]);
                    else if (dynamic_cast<GeometryOutport*>(outport) && dynamic_cast<GeometryInport*>(inports[i]))
                        processorNetwork_->addConnection(outport, inports[i]);
                }
            }
        }
    }
    processorNetwork_->unlock();
    processorNetwork_->setBroadcastModification(true);
}

void NetworkEditor::removeInspectorNetwork(Port* port) {   
    std::string portPrefix = port->getProcessor()->getIdentifier()+":"+port->getIdentifier();
    removeExternalNetwork(portPrefix);    
}

void NetworkEditor::addPortInspector(Port* port, QPointF pos) {
    //TODO: allow to define inspectors in module
    if((dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettings()->getPropertyByIdentifier("enablePortInspectors"))->get())){
        if (port->getProcessor()->isInitialized()) {
            if (dynamic_cast<ImageOutport*>(port)) {
                addInspectorNetwork(port, ivec2(pos.x(), pos.y()),
                                    IVW_DIR+"data/workspaces/portinspectors/imageportinspector.inv");
                processorNetwork_->setModified(true);
                processorNetworkEvaluator_->requestEvaluate();
                return;
			} 
			if (dynamic_cast<VolumeOutport*>(port)) {
				addInspectorNetwork(port, ivec2(pos.x(), pos.y()),
					IVW_DIR+"data/workspaces/portinspectors/volumeportinspector.inv");
				processorNetwork_->setModified(true);
				processorNetworkEvaluator_->requestEvaluate();
				return;
			} 
			if (dynamic_cast<GeometryOutport*>(port)) {
				addInspectorNetwork(port, ivec2(pos.x(), pos.y()),
					IVW_DIR+"data/workspaces/portinspectors/geometryportinspector.inv");
				processorNetwork_->setModified(true);
				processorNetworkEvaluator_->requestEvaluate();
				return;
			}
        }
    }
}

void NetworkEditor::hoverPortTimeOut() {
    addPortInspector(inspectedPort_, QCursor::pos());
}

void NetworkEditor::errorString(QString str){
    LogError(str.toLocal8Bit().constData());
}

void NetworkEditor::workerThreadReset(){
    workerThread_ = NULL;
}

void NetworkEditor::workerThreadQuit(){
    if(workerThread_){
        workerThread_->quit();
        workerThread_->wait();
        workerThreadReset();
    }
}

void NetworkEditor::cacheProcessorProperty(Processor* p){
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    if(std::find(processors.begin(), processors.end(), p) != processors.end()){
        PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
        propertyListWidget_->cacheProcessorPropertiesItem(p);
    }
}

////////////////////////////////////////////////////////
//   LOAD AND GET SNAPSHOT FROM EXTERNAL NETWORK      //
////////////////////////////////////////////////////////

void NetworkEditor::addExternalNetwork(std::string fileName, std::string identifierPrefix, ivec2 pos, unsigned int networkEditorFlags , ivec2 canvasSize) {
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
            processorNetworkEvaluator_->registerCanvas(canvasProcessor->getCanvas(), canvasProcessor->getIdentifier()); //register is required because identifier is modified.
        }
    }

    std::vector<PortConnection*> connections = processorNetwork->getPortConnections();
    for (size_t i=0; i<connections.size(); i++) {
        PortConnection* connection = connections[i];
        Outport* outport = connection->getOutport();
        Inport* inport = connection->getInport();
        // first remove the connection from the loaded network to avoid an already connected warning
        processorNetwork->removeConnection(outport, inport);
        addConnection(outport, inport);
    }
    
    CameraProperty* externalNetworkCamera = 0;
    std::vector<ProcessorLink*> links = processorNetwork->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {        
        addLink(links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        ProcessorLink* link = processorNetwork->getProcessorLink(links[i]->getSourceProcessor(), links[i]->getDestinationProcessor());
        std::vector<PropertyLink*> propertyLinks = link->getPropertyLinks();
        for (size_t j=0; j<propertyLinks.size(); j++) {
            Property* srcProp = propertyLinks[j]->getSourceProperty();
            Property* dstProp = propertyLinks[j]->getDestinationProperty();
            link->addPropertyLinks(srcProp, dstProp);
            if (!externalNetworkCamera) externalNetworkCamera = dynamic_cast<CameraProperty*>(srcProp);
            if (!externalNetworkCamera) externalNetworkCamera = dynamic_cast<CameraProperty*>(dstProp);
        }
    }

    //TODO: link any camera in current network with externalNetworkCamera

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
    BoolProperty* soundProperty = dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettings()->getPropertyByIdentifier("enableSound"));
    bool isSoundEnabled = soundProperty->get();
    if (isSoundEnabled) soundProperty->set(false);

    std::vector<std::string> canvasSnapShotFiles;
    std::string directory = URLParser::getFileDirectory(externalNetworkFile);
    std::string workSpaceName = URLParser::getFileNameWithExtension(externalNetworkFile);
    std::string newFileName = URLParser::replaceFileExtension(workSpaceName, "png");   
    processorNetwork_->setBroadcastModification(false);    
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0;i<processors.size();i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix)!=std::string::npos) {
            CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processors[i]);
            if (canvasProcessor) {
                std::string snapShotFilePath = directory + "snapshot_" + canvasProcessor->getIdentifier() + newFileName;
                canvasSnapShotFiles.push_back(snapShotFilePath);
                canvasProcessor->createSnapshot(snapShotFilePath.c_str());
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
    for (size_t i=0;i<processors.size();i++) {
        if (processors[i]->getIdentifier().find(identifierPrefix)!=std::string::npos) {
            removeProcessor(processors[i]);
        }
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
        Processor* outProcessor = linkGraphicsItems_[i]->getOutProcessorGraphicsItem()->getProcessor();
        Processor* inProcessor = linkGraphicsItems_[i]->getInProcessorGraphicsItem()->getProcessor();
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
                } else if (startPort_ && dynamic_cast<Inport*>(startPort_)) {
                    // click on inport: disconnect if connected
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

                        e->accept();
                        
                    }

                } else {
                    // click on processor but not on port: show property widgets
                    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
                    propertyListWidget_->setMultiSelect(false);
                    propertyListWidget_->showProcessorProperties(startProcessor_->getProcessor());
                    QGraphicsScene::mousePressEvent(e);
                }

            } else if (e->modifiers() == Qt::ControlModifier) {
                if (isLinkDisplayEnabled()) {
                    // ctrl modifier pressed: edit link
                    QRectF processorRect = startProcessor_->mapToScene(processorRect).boundingRect();
                    linkCurve_ = new LinkGraphicsItem(processorRect.center(), e->scenePos());
                    addItem(linkCurve_);
                    linkCurve_->show();
                }
                else {
                    LogWarn("Enable Display links in Settings to create links")
                }
                e->accept();
            }
        } else {
            // no processor selected
            PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
            propertyListWidget_->removeAllProcessorProperties();
            QGraphicsScene::mousePressEvent(e);
        }
    }
    else if (e->button() == Qt::RightButton) {
        startProcessor_ = getProcessorGraphicsItemAt(e->scenePos());
        if (startProcessor_) {
            PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
            propertyListWidget_->showProcessorProperties(startProcessor_->getProcessor());
            QGraphicsScene::mousePressEvent(e);
        }
    }
}

void NetworkEditor::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (connectionCurve_) {
        // connection drag mode
        connectionCurve_->setEndPoint(e->scenePos());

        endProcessor_ = getProcessorGraphicsItemAt(e->scenePos());
        if (endProcessor_) {
            Port* port = endProcessor_->getSelectedPort(e->scenePos());
            if (port && port!=startPort_) {
                Inport* inport = dynamic_cast<Inport*>(port);
                if(inport && inport->canConnectTo(startPort_) && !inport->isConnectedTo(dynamic_cast<Outport*>(startPort_)))
                    connectionCurve_->setBorderColor(Qt::green);	
                else
                    connectionCurve_->setBorderColor(Qt::red);	
            }
            else
                connectionCurve_->resetBorderColors();
        }
        else
            connectionCurve_->resetBorderColors();

        endProcessor_ = NULL;

        connectionCurve_->update();
        e->accept();
    }
    
    else if (linkCurve_) {
        // link drag mode
        QPointF center = startProcessor_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        linkCurve_->update();
        e->accept();
    }

    else if (e->button()==Qt::NoButton) {
        // port inspector hover effect
        Port* port = 0;
        ProcessorGraphicsItem* processor = getProcessorGraphicsItemAt(e->scenePos());
        if (processor) {
            port = processor->getSelectedPort(e->scenePos());
            // if we hover over an inport get its connected outport instead
            Inport* inport = dynamic_cast<Inport*>(port);
            if (inport)
                port = inport->getConnectedOutport();
        } else {
            ConnectionGraphicsItem* connection = getConnectionGraphicsItemAt(e->scenePos());
            LinkConnectionGraphicsItem* link = getLinkGraphicsItemAt(e->scenePos());
            if (connection)
                port = connection->getOutport();
            else if (link) {
                ProcessorGraphicsItem* outProcessorGraphicsItem = link->getOutProcessorGraphicsItem();
                ProcessorGraphicsItem* inProcessorGraphicsItem = link->getInProcessorGraphicsItem();

                Processor* outProcessor = outProcessorGraphicsItem->getProcessor();
                Processor* inProcessor = inProcessorGraphicsItem->getProcessor();               

                ProcessorLink* processorLink = processorNetwork_->getProcessorLink(outProcessor, inProcessor);

                if (processorLink) {
                    std::string outId = outProcessor->getIdentifier();
                    std::string inId = inProcessor->getIdentifier();
                    QString title;

                    std::vector<PropertyLink*> processedLinks;
                    std::vector<PropertyLink*> propertyLinks = processorLink->getPropertyLinks();
                    for (size_t i=0; i<propertyLinks.size(); i++) {
                        if (std::find(processedLinks.begin(), processedLinks.end(), propertyLinks[i])==processedLinks.end()) {
                            Property* srcProperty = propertyLinks[i]->getSourceProperty();
                            Property* dstProperty = propertyLinks[i]->getDestinationProperty(); 
                            PropertyLink* pairLink = processorLink->getBidirectionalPair(srcProperty, dstProperty);
                            if (pairLink) 
                                processedLinks.push_back(pairLink); 
                            processedLinks.push_back(propertyLinks[i]);
                            std::string linkInfo = outId + ":" + srcProperty->getDisplayName() + " - " + inId + ":" + dstProperty->getDisplayName() ;
                            title+=QString::fromStdString(linkInfo+"\n");
                        }
                    }
                    link->setToolTip(title);

                }

            }
        }

        if (port) {
            if (inspectedPort_ != port) {
                if (inspectedPort_)
                    removeInspectorNetwork(inspectedPort_);
                inspectedPort_ = port;
                hoverTimer_.start(500);
            }
        } else {
            if (inspectedPort_) {
                hoverTimer_.stop();
                removeInspectorNetwork(inspectedPort_);
                inspectedPort_ = 0;
            }
        }
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
            if(endPort_){
                Inport* inport = dynamic_cast<Inport*>(endPort_);
                if (inport && inport->canConnectTo(startPort_)) {
                    // MultiDataInports can have several connections
                    SingleInport* singleInport = dynamic_cast<SingleInport*>(inport);
                    if(inport->isConnected() && singleInport != NULL)
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
        if (endProcessor_) {
            addLink(startProcessor_->getProcessor(), endProcessor_->getProcessor());
            LinkConnectionGraphicsItem* linkGraphicsItem = getLinkGraphicsItem(startProcessor_->getProcessor(), endProcessor_->getProcessor());
            showLinkDialog(linkGraphicsItem);
        }

        startProcessor_ = 0; 
        endProcessor_ = 0;
        e->accept();

    } else if (startProcessor_){
        // move processor
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
            if (processorGraphicsItem && gridSnapping_)
                processorGraphicsItem->setPos(snapToGrid(processorGraphicsItem->pos()));				
		}
        QGraphicsScene::mouseReleaseEvent(e);
    } else if(selectedItems().size()>1){
        //Show multiple processor widgets
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
        PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
        std::vector<Processor*> processors;
        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            if (dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i])) {
                ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
                processors.push_back(processorGraphicsItem->getProcessor());
            }
        }
        propertyListWidget_->showProcessorProperties(processors);
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
        Port* selectedPort = processorGraphicsItem->getSelectedPort(e->scenePos());
        if (selectedPort && dynamic_cast<Outport*>(selectedPort)) {
            /*
            // Port context menu
            QMenu menu;
            QAction* inspectorAction = menu.addAction("Pin inspector");
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
            */
        } else {
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
            if (result == renameAction) {
                processorGraphicsItem->editProcessorName();
            } else if (result == deleteAction) {
                Processor* processor = processorGraphicsItem->getProcessor();
                removeProcessor(processor);
            } else if (showAction && result == showAction) {
                if (showAction->isChecked())
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
        if (isLinkDisplayEnabled()) {
            QMenu menu;
            QAction* linkAction = menu.addAction(tr("Link properties"));
            QAction* deleteAction = menu.addAction("Delete");
            QAction* result = menu.exec(QCursor::pos());
            if (result == deleteAction)
                removeLink(linkGraphicsItem);

            else if (result == linkAction) {
                Processor* inProcessor = linkGraphicsItem->getInProcessorGraphicsItem()->getProcessor();
                Processor* outProcessor = linkGraphicsItem->getOutProcessorGraphicsItem()->getProcessor();
                ProcessorLink* processorLink = processorNetwork_->getProcessorLink(inProcessor, outProcessor);
                if (processorLink)
                    processorLink->autoLinkPropertiesByType();
            }
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

		ConnectionGraphicsItem* connectionItem = getConnectionGraphicsItemAt(e->scenePos());
		if (connectionItem && !oldConnectionTarget_){ //< New connection found
			QString className;
			ProcessorDragObject::decode(e->mimeData(), className);
			Processor* processor = static_cast<Processor*>(ProcessorFactory::getRef().create(className.toLocal8Bit().constData()));
			QColor inputColor = Qt::red, outputColor = Qt::red;
			// Try to find a match between connection outport and one of the new processors inports
			for (size_t i = 0; i < processor->getInports().size(); ++i) { 
				if(processor->getInports().at(i)->canConnectTo(connectionItem->getOutport())) {
					inputColor = Qt::green;
					break;
				}
			}
			// Try to find a match between connection inport and one of the new processors outports
			for (size_t i = 0; i < processor->getOutports().size(); ++i) {
				if(connectionItem->getInport()->canConnectTo(processor->getOutports().at(i))) {
					outputColor = Qt::green;
					break;
				}
			}
			connectionItem->setBorderColors(inputColor, outputColor);		
			oldConnectionTarget_ = connectionItem;
		} else if (connectionItem) { //< Move event on active connection
			oldConnectionTarget_->setMidPoint(e->scenePos());
		} else if (oldConnectionTarget_ && !connectionItem){ //< Connection no longer targeted
			oldConnectionTarget_->clearMidPoint();
			oldConnectionTarget_ = NULL;
		} else if (!connectionItem) { //< Processor replacement
			ProcessorGraphicsItem* processorItem = getProcessorGraphicsItemAt(e->scenePos());
			if (processorItem && !oldProcessorTarget_) { //< New processor found
				QString className;
				ProcessorDragObject::decode(e->mimeData(), className);
				processorItem->setSelected(true);
				oldProcessorTarget_ = processorItem;
			} else if ( !processorItem && oldProcessorTarget_) { //< Processor no longer targeted
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

			// Check for collisions
			if(oldConnectionTarget_)
				placeProcessorOnConnection(processorGraphicsItem, oldConnectionTarget_);
            else if(oldProcessorTarget_)
                placeProcessorOnProcessor(processorGraphicsItem, oldProcessorTarget_);
		}
	}
}

void NetworkEditor::placeProcessorOnConnection(ProcessorGraphicsItem* processorItem, ConnectionGraphicsItem* connectionItem) {
	std::vector<Inport*> inports = processorItem->getProcessor()->getInports();
	std::vector<Outport*> outports = processorItem->getProcessor()->getOutports();
	Inport* connectionInport = connectionItem->getInport();
	Outport* connectionOutport = connectionItem->getOutport();

	// Clear oldDragTarget
	oldConnectionTarget_->clearMidPoint();
	oldConnectionTarget_ = NULL;

    processorNetwork_->lock();

	// Remove old connection
	removeConnection(connectionItem);

	for (size_t i = 0; i < inports.size(); ++i) {
		if(inports.at(i)->canConnectTo(connectionOutport)) {
			// Create new connection connectionOutport-processorInport
			addConnection(connectionOutport, inports.at(i));
			break;
		}
	}

	for (size_t i = 0; i < outports.size(); ++i) {
		if(connectionInport->canConnectTo(outports.at(i))) {
			// Create new connection processorOutport-connectionInport
			addConnection(outports.at(i), connectionInport);
			break;
		}
	}

    processorNetwork_->unlock();

    if(processorNetwork_->isModified())
        processorNetworkEvaluator_->requestEvaluate();	
}

void NetworkEditor::placeProcessorOnProcessor(ProcessorGraphicsItem* processorItem, ProcessorGraphicsItem* oldProcessorItem){
    std::vector<Inport*> inports = processorItem->getProcessor()->getInports();
    std::vector<Outport*> outports = processorItem->getProcessor()->getOutports();

    std::vector<Inport*> oldInports = oldProcessorItem->getProcessor()->getInports();
    std::vector<Outport*> oldOutports = oldProcessorItem->getProcessor()->getOutports();
    
    processorNetwork_->lock();

    std::vector<std::pair<Outport*, Inport*> > newConnections;

    for (size_t i = 0; i < std::min(inports.size(), oldInports.size()); ++i) {
        if(inports.at(i)->canConnectTo(oldInports.at(i)->getConnectedOutport())) {
            // Save new connection connectionOutportoldInport-processorInport
            newConnections.push_back(std::make_pair(oldInports.at(i)->getConnectedOutport(), inports.at(i)));
        }
    }

    for (size_t i = 0; i < std::min(outports.size(), oldOutports.size()); ++i) {
        std::vector<Inport*> connectionInports = oldOutports.at(i)->getConnectedInports();
        for (size_t j = 0; j < connectionInports.size(); ++j) {
            if(connectionInports.at(j)->canConnectTo(outports.at(i))) {
                // Save new connection processorOutport-connectionInport
                newConnections.push_back(std::make_pair(outports.at(i), connectionInports.at(j)));
            }
        }
    }

    //Remove old processor
    removeProcessor(oldProcessorItem->getProcessor());

    //Create all new connections
    for (size_t i = 0; i < newConnections.size(); ++i) {
        addConnection(newConnections.at(i).first, newConnections.at(i).second);
    }

    processorNetwork_->unlock();

    if(processorNetwork_->isModified())
        processorNetworkEvaluator_->requestEvaluate();	
}

///////////////////////////////
//   SERIALIZATION METHODS   //
///////////////////////////////
void NetworkEditor::clearNetwork() {
    processorNetwork_->lock();
    ResourceManager::instance()->clearAllResources();
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
    return true;
}

bool NetworkEditor::loadNetwork(std::string fileName) {
    workerThreadQuit();

    // first we clean the current network
    clearNetwork();

    // then we lock the network that no evaluations are triggered during the deserialization
    processorNetwork_->lock();

    // then we deserialize processor network
    IvwDeserializer xmlDeserializer(fileName);
    try {
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
        addProcessorRepresentations(processors[i], QPointF(meta->getPosition().x, meta->getPosition().y), meta->isVisible(), false, false);
    }

    // add connections
    std::vector<PortConnection*> connections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<connections.size(); i++)
        addConnectionGraphicsItem(connections[i]->getOutport(), connections[i]->getInport());

    // add link graphics items
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++)
        addLinkGraphicsItem(links[i]->getDestinationProcessor(), links[i]->getSourceProcessor());

    // flag the network's modified flag, unlock it and initiate evaluation
    processorNetwork_->setModified(true);
    processorNetwork_->unlock();
    // show all processor widgets that where hidden on network load
    for (size_t i=0; i<processors.size(); i++) {
        if (processors[i]->hasProcessorWidget()){
            ProcessorMetaData* meta = dynamic_cast<ProcessorMetaData*>(processors[i]->getMetaData("ProcessorMetaData"));
            if(meta->isVisible())
                processors[i]->getProcessorWidget()->show();
        }
    }

    // create all property (should be all non-visible) widgets in a thread (as it can take a long time to create them)
    workerThread_ = new QThread;
    ProcessorWorkerQt* worker = new ProcessorWorkerQt(processors);
    worker->moveToThread(workerThread_);
    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(workerThread_, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(nextProcessor(Processor*)), this, SLOT(cacheProcessorProperty(Processor*)));
    connect(worker, SIGNAL(finished()), workerThread_, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(workerThread_, SIGNAL(finished()), workerThread_, SLOT(deleteLater()));
    connect(workerThread_, SIGNAL(finished()), this, SLOT(workerThreadReset()));
    workerThread_->start();

    return true;
}

void NetworkEditor::updatePropertyListWidget(){
    QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();
    PropertyListWidget* propertyListWidget_ = PropertyListWidget::instance();
    for (int i=0; i<selectedGraphicsItems.size(); i++) {
        ProcessorGraphicsItem* processorGraphicsItem = dynamic_cast<ProcessorGraphicsItem*>(selectedGraphicsItems[i]);
        if (processorGraphicsItem) {
            propertyListWidget_->showProcessorProperties(processorGraphicsItem->getProcessor());
        }
    }
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
    Property* prop = InviwoApplication::getPtr()->getSettings()->getPropertyByIdentifier("displayLinks");
    ivwAssert(prop!=0, "Display Links property not found in settings");

    BoolProperty* displayLinkProperty = dynamic_cast<BoolProperty*>( prop );
    return displayLinkProperty->get();   
}

void ProcessorWorkerQt::process() {
    for(std::vector<Processor*>::iterator it = processors_.begin(); it != processors_.end(); ++it){
        emit nextProcessor(*it);
    }
    emit finished();
}

} // namespace