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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/util/msvc-memleak-includes.h>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/processors/progressbarowner.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {


std::map<ProcessorNetwork*,ProcessorNetworkEvaluator*> ProcessorNetworkEvaluator::processorNetworkEvaluators_;

ProcessorNetworkEvaluator::ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork)
    : processorNetwork_(processorNetwork) { 
    registeredCanvases_.clear();
    initializeNetwork();
    defaultContext_ = 0;
    eventInitiator_ = 0;
  //  linkEvaluator_  = 0;
    evaulationQueued_ = false;
    evaluationDisabled_ = false;
    ivwAssert(processorNetworkEvaluators_.find(processorNetwork) == processorNetworkEvaluators_.end() , "A ProcessorNetworkEvaluator for the given ProcessorNetwork is already created");
    
    processorNetworkEvaluators_[processorNetwork] = this;
}

ProcessorNetworkEvaluator::~ProcessorNetworkEvaluator() {
    std::map<ProcessorNetwork*,ProcessorNetworkEvaluator*>::iterator it = processorNetworkEvaluators_.find(processorNetwork_);
    if(it != processorNetworkEvaluators_.end())
        processorNetworkEvaluators_.erase(it);
  //  delete linkEvaluator_;
}


void ProcessorNetworkEvaluator::activateDefaultRenderContext() {
    defaultContext_->activate();
}

void ProcessorNetworkEvaluator::initializeNetwork() {
    ivwAssert(processorNetwork_!=0, "processorNetwork_ not initialized, call setProcessorNetwork()");
    // initialize network
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        if (!processors[i]->isInitialized())
            processors[i]->initialize();
   // linkEvaluator_  = new LinkEvaluator();
}

void ProcessorNetworkEvaluator::registerCanvas(Canvas* canvas, std::string associatedProcessName) {    
    if (std::find(registeredCanvases_.begin(), registeredCanvases_.end(), canvas)!=registeredCanvases_.end()) {
        //already registered, hence force deregisterCanvas
        deregisterCanvas(canvas);
    }
    registeredCanvases_.push_back(canvas);
    canvas->setNetworkEvaluator(this);
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (size_t i=0; i<canvasProcessors.size(); i++) {
        if (canvasProcessors[i]->getIdentifier() == associatedProcessName) {
            canvasProcessors[i]->setCanvas(canvas);            
        }
    }
}

void ProcessorNetworkEvaluator::deregisterCanvas(Canvas *canvas) {
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (unsigned int i=0; i<canvasProcessors.size(); i++) {
        Canvas* curCanvas = canvasProcessors[i]->getCanvas();
        if (curCanvas==canvas) {
            if (std::find(registeredCanvases_.begin(), registeredCanvases_.end(), canvas)!=registeredCanvases_.end()) {
                canvas->setNetworkEvaluator(0);
                canvasProcessors[i]->setCanvas(0);
                registeredCanvases_.erase(std::remove(registeredCanvases_.begin(), registeredCanvases_.end(),
                                          canvas), registeredCanvases_.end());
                return;
            }
        }
    }
    LogError("Trying to deregister unregistered Canvas.");
}


void ProcessorNetworkEvaluator::saveSnapshotAllCanvases(std::string dir, std::string default_name, std::string ext){
    std::vector<inviwo::CanvasProcessor*> pv = processorNetwork_->getProcessorsByType<inviwo::CanvasProcessor>();
    int i = 0;
    for(std::vector<inviwo::CanvasProcessor*>::iterator it = pv.begin(); it != pv.end(); it++){
        std::stringstream ss;
        if (default_name == "" || default_name == "UPN")
            ss << (*it)->getIdentifier();
        else
            ss << default_name << i+1;
        std::string path(dir + ss.str() + ext);
        LogInfo("Saving canvas to: " + path);
        (*it)->createSnapshot((path).c_str());
        ++i;
    }
}

bool ProcessorNetworkEvaluator::hasBeenVisited(Processor* processor) {
    for (size_t i=0; i<processorsVisited_.size(); i++)
        if (processorsVisited_[i] == processor)
            return true;
    return false;
}

std::vector<Processor*> ProcessorNetworkEvaluator::getDirectPredecessors(Processor* processor) {
    std::vector<Processor*> predecessors;
    std::vector<Inport*> inports = processor->getInports();
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Outport* connectedOutport = portConnections[j]->getOutport();
                predecessors.push_back(connectedOutport->getProcessor());
            }
        }
    }
    return predecessors;
}

void ProcessorNetworkEvaluator::traversePredecessors(Processor* processor) {
    if (!hasBeenVisited(processor)) {
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++)
            traversePredecessors(directPredecessors[i]);
        processorsSorted_.push_back(processor);
    }
}

void ProcessorNetworkEvaluator::determineProcessingOrder() {
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    std::vector<Processor*> endProcessors;
    for (size_t i=0; i<processors.size(); i++)
        if (processors[i]->isEndProcessor())
            endProcessors.push_back(processors[i]);

    // perform topological sorting and store processor order
    // in processorsSorted_
    processorsSorted_.clear();
    processorsVisited_.clear();
    for (size_t i=0; i<endProcessors.size(); i++)
        traversePredecessors(endProcessors[i]);
}

void ProcessorNetworkEvaluator::propagateInteractionEvent(Processor* processor, InteractionEvent* event) {
	if (!hasBeenVisited(processor)) {
		processorsVisited_.push_back(processor);
		std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
		for (size_t i=0; i<directPredecessors.size(); i++) {
			if (directPredecessors[i]->hasInteractionHandler())
				directPredecessors[i]->invokeInteractionEvent(event);
			propagateInteractionEvent(directPredecessors[i], event);
		}
	}
}

void ProcessorNetworkEvaluator::propagateInteractionEvent(Canvas* canvas, InteractionEvent* event) {
	// find the canvas processor from which the event was emitted
	eventInitiator_=0;
	std::vector<Processor*> processors = processorNetwork_->getProcessors();
    CanvasProcessor* canvasProcessor = NULL;
	for (size_t i=0; i<processors.size(); i++) {
        canvasProcessor = dynamic_cast<CanvasProcessor*>(processors[i]);
		if (canvasProcessor && canvasProcessor->getCanvas()==canvas) {
				eventInitiator_ = processors[i];
				i = processors.size();
		}
	}

	processorsVisited_.clear();
	propagateInteractionEvent(eventInitiator_, event);
    eventInitiator_ = 0;
}

/// /NEW ------------------------------------------------------------

bool ProcessorNetworkEvaluator::isPortConnectedToProcessor(Port* port, Processor *processor) {
    bool isConnected = false;
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();

    std::vector<Outport*> outports = processor->getOutports();   
    for (size_t i=0; i<outports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curOutport = portConnections[j]->getOutport();
            if (curOutport == outports[i]) {
                const Port* connectedInport = portConnections[j]->getInport();
                if (connectedInport == port) {
                    isConnected = true;
                    break;
                }
            }
        }
    }

    if (isConnected) return isConnected;

    std::vector<Inport*> inports = processor->getInports();   
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Outport* connectedOutport = portConnections[j]->getOutport();
                if (connectedOutport == port) {
                    isConnected = true;
                    break;
                }
            }
        }
    }

    return isConnected;

}

Processor* ProcessorNetworkEvaluator::retrieveCanvasProcessor(Canvas* canvas) {
    // find the canvas processor which contains the canvas
    Processor* processor = NULL;
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    CanvasProcessor* canvasProcessor = NULL;
    for (size_t i=0; i<processors.size(); i++) {
        canvasProcessor = dynamic_cast<CanvasProcessor*>(processors[i]);
        if (canvasProcessor && canvasProcessor->getCanvas()==canvas) {
                processor = processors[i];
                i = processors.size();
        }
    }
    
    return processor;
}

void ProcessorNetworkEvaluator::propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent) {
    if (processorNetwork_->islocked()) return;

    // avoid continues evaluation when port change
    processorNetwork_->lock();
    
    // find the canvas processor from which the event was emitted
	eventInitiator_= 0;
    eventInitiator_= retrieveCanvasProcessor(canvas);
    ivwAssert(eventInitiator_!=0, "Invalid resize event encountered.");

    // propagate size of canvas to all preceding processors through port
    // event initiator is a canvas processor, hence one ImageInport should exist
    ImageInport* imageInport = static_cast<ImageInport*>(eventInitiator_->getInports()[0]);    
    imageInport->changeDataDimensions(resizeEvent);

    // enable network evaluation again
    processorNetwork_->unlock();

    // TODO: remove this invalidate
    // instead dimension property of event initiator (CanvasProcessor) should be invalid
    eventInitiator_->invalidate(PropertyOwner::INVALID_OUTPUT);
    eventInitiator_ = 0;
}

std::vector<PropertyLink*> ProcessorNetworkEvaluator::getConnectedPropertyLinks(Property* property) {
    std::vector<PropertyLink*> propertyLinks;
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        std::vector<PropertyLink*> plinks = links[i]->getPropertyLinks();
        for (size_t j=0; j<plinks.size(); j++) {
            if (plinks[j]->getSourceProperty()==property || plinks[j]->getDestinationProperty()==property) {
                propertyLinks.push_back(plinks[j]);
            }
        }
    }
    return propertyLinks;
}

std::vector<Property*> ProcessorNetworkEvaluator::getLinkedProperties(Property* property) {
    std::vector<Property*> connectedProperties;
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        std::vector<PropertyLink*> plinks = links[i]->getPropertyLinks();
        for (size_t j=0; j<plinks.size(); j++) {
            if (plinks[j]->getSourceProperty()==property) {
                connectedProperties.push_back(plinks[j]->getDestinationProperty());
            }            
        }
    }
    return connectedProperties;
}

bool ProcessorNetworkEvaluator::hasBeenVisited(Property* property) {
    if (std::find(propertiesVisited_.begin(), propertiesVisited_.end(), property)== propertiesVisited_.end())
        return false;
    return true;
}

std::vector<ProcessorLink*> ProcessorNetworkEvaluator::getSortedProcessorLinks() {
    std::vector<ProcessorLink*> unsortedProcessorLinks = processorNetwork_->getProcessorLinks();
    std::vector<ProcessorLink*> sortedProcessorLinks;

    ProcessorLink* nextInvalidLink = 0;

    //Find initial invalid link (whose property is currently modified)
    for (size_t i=0; i<unsortedProcessorLinks.size(); i++) { 
        if (!unsortedProcessorLinks[i]->isValid()) { 
            nextInvalidLink = unsortedProcessorLinks[i];
            break;
        }
    }

    if (!nextInvalidLink)  return sortedProcessorLinks; //return empty list

    //Find link connected to the invalid link.
    //If it does not exist pick any unsorted processor link and repeat.    
    while (sortedProcessorLinks.size()!=unsortedProcessorLinks.size()) {        
        sortedProcessorLinks.push_back(nextInvalidLink);
        
        Processor* srcProc = nextInvalidLink->getSourceProcessor();
        Processor* dstProc = nextInvalidLink->getDestinationProcessor();

        //Find link connected to the invalid link.
        nextInvalidLink = 0;
        for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
            if ( std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(), unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                std::vector<PropertyLink*> propertyLinks = unsortedProcessorLinks[i]->getPropertyLinks();                
                for (size_t j=0; j<propertyLinks.size(); j++) { 
                    if (propertyLinks[j]->getSourceProperty()->getOwner() == dstProc ||
                        propertyLinks[j]->getSourceProperty()->getOwner() == srcProc) {
                        nextInvalidLink = unsortedProcessorLinks[i];
                        break;
                    }
                }
            }
        }

        //Pick any unsorted link.
        if (!nextInvalidLink) {
            for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
                if ( std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(), unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                    if (!unsortedProcessorLinks[i]->isValid()) { 
                        nextInvalidLink = unsortedProcessorLinks[i];
                        break;
                    }
                }
            }
        }

        //there is no more invalid links
        if (!nextInvalidLink) break;        
    }

    return sortedProcessorLinks;
}

void ProcessorNetworkEvaluator::notifyInvalidationEnd(Processor* p){
    processorNetwork_->notifyInvalidationEnd(p);
    p->removeObserver(this);
    if(evaulationQueued_){
        evaulationQueued_ = false;
        requestEvaluate();
    }
}

void ProcessorNetworkEvaluator::disableEvaluation(){
    evaluationDisabled_ = true;
}

void ProcessorNetworkEvaluator::enableEvaluation(){
    evaluationDisabled_ = false;
    if(evaulationQueued_){
        evaulationQueued_ = false;
        requestEvaluate();
    }
}


ProcessorNetworkEvaluator* ProcessorNetworkEvaluator::getProcessorNetworkEvaluatorForProcessorNetwork(ProcessorNetwork *network){
    std::map<ProcessorNetwork*,ProcessorNetworkEvaluator*>::iterator it = processorNetworkEvaluators_.find(network);
    if(it == processorNetworkEvaluators_.end())
        return new ProcessorNetworkEvaluator(network);
    return it->second;
}

void ProcessorNetworkEvaluator::requestEvaluate() {
    if (processorNetwork_->islocked())
        return;

    //evaluation has been triggered but not performed yet
    if(evaulationQueued_)
        return;

    //evaluation disabled
    if (evaluationDisabled_){
        evaulationQueued_ = true;
        return;
    }

    //wait for invalidation to finish before evaluating
    if (processorNetwork_->isInvalidating()){
        evaulationQueued_ = true;
        processorNetwork_->getInvalidationInitiator()->addObserver(this);
        return;
    }
    
    //if we haven't returned yet, perform evaluation of the network
    evaluate();
}

void ProcessorNetworkEvaluator::evaluate() {
    // lock processor network to avoid concurrent evaluation
    processorNetwork_->lock();    

    //perform linking
    std::vector<ProcessorLink*> processorLinks = getSortedProcessorLinks();        
    for (size_t i=0; i<processorLinks.size(); i++) {         
        if (!processorLinks[i]->isValid())
            processorLinks[i]->evaluate(&linkEvaluator_);
    } 
  
    // if the processor network has changed determine the new processor order
    if (processorNetwork_->isModified()) {
        defaultContext_->activate();
        initializeNetwork();
        determineProcessingOrder();
        processorNetwork_->setModified(false);
    }
 
    std::vector<Processor*>::iterator it;
    defaultContext_->activate();
    for (it = processorsSorted_.begin(); it != processorsSorted_.end(); ++it) {
        if (!(*it)->isValid()) {
            if ((*it)->isReady()){
                // re-initialize resources (e.g., shaders) if necessary
                if ((*it)->getInvalidationLevel() >= PropertyOwner::INVALID_RESOURCES)
                    (*it)->initializeResources();

                // do the actual processing
                (*it)->process();
            }
        }
    }       
    for (it = processorsSorted_.begin(); it != processorsSorted_.end(); ++it) {
        // validate processor
        (*it)->setValid();
    }

    // unlock processor network to allow next evaluation
    processorNetwork_->unlock();
}

} // namespace
