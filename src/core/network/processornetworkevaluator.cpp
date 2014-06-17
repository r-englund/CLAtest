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
 * Main file authors: Timo Ropinski, Erik Sundén, Sathish Kottravel
 *
 *********************************************************************************/

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
    evaulationQueued_ = false;
    evaluationDisabled_ = false;
    ivwAssert(processorNetworkEvaluators_.find(processorNetwork) == processorNetworkEvaluators_.end() ,
              "A ProcessorNetworkEvaluator for the given ProcessorNetwork is already created");
    processorNetworkEvaluators_[processorNetwork] = this;
    processorNetwork_->addObserver(this);
}

ProcessorNetworkEvaluator::~ProcessorNetworkEvaluator() {
    std::map<ProcessorNetwork*,ProcessorNetworkEvaluator*>::iterator it = processorNetworkEvaluators_.find(processorNetwork_);

    if (it != processorNetworkEvaluators_.end())
        processorNetworkEvaluators_.erase(it);
}

void ProcessorNetworkEvaluator::setDefaultRenderContext(Canvas* canvas) { 
    defaultContext_ = canvas;
    defaultContext_->setNetworkEvaluator(this);
}

void ProcessorNetworkEvaluator::activateDefaultRenderContext() const {
    if (defaultContext_)
        defaultContext_->activate();
}

void ProcessorNetworkEvaluator::initializeNetwork() {
    ivwAssert(processorNetwork_!=0, "processorNetwork_ not initialized, call setProcessorNetwork()");
    // initialize network
    std::vector<Processor*> processors = processorNetwork_->getProcessors();

    for (size_t i=0; i<processors.size(); i++)
        if (!processors[i]->isInitialized())
            processors[i]->initialize();
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
        if (canvasProcessors[i]->getIdentifier() == associatedProcessName)
            canvasProcessors[i]->setCanvas(canvas);
    }
}

void ProcessorNetworkEvaluator::deregisterCanvas(Canvas* canvas) {
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


void ProcessorNetworkEvaluator::saveSnapshotAllCanvases(std::string dir, std::string default_name, std::string ext) {
    std::vector<inviwo::CanvasProcessor*> pv = processorNetwork_->getProcessorsByType<inviwo::CanvasProcessor>();
    int i = 0;

    for (std::vector<inviwo::CanvasProcessor*>::iterator it = pv.begin(); it != pv.end(); it++) {
        std::stringstream ss;

        if (default_name == "" || default_name == "UPN")
            ss << (*it)->getIdentifier();
        else
            ss << default_name << i+1;

        std::string path(dir + ss.str() + ext);
        LogInfo("Saving canvas to: " + path);
        (*it)->saveImageLayer((path).c_str());
        ++i;
    }
}

bool ProcessorNetworkEvaluator::hasBeenVisited(Processor* processor) {
    for (size_t i=0; i<processorsVisited_.size(); i++)
        if (processorsVisited_[i] == processor)
            return true;

    return false;
}

std::set<Processor*> ProcessorNetworkEvaluator::getDirectPredecessors(Processor* processor, Event* event) {
    std::set<Processor*> predecessors;
    const std::vector<Inport*>& inports = processor->getInports(event);

    for (std::vector<Inport*>::const_iterator portIt = inports.begin(), portItEnd = inports.end(); portIt!=portItEnd; ++portIt) {
        if (!(*portIt)->isConnected())
            continue;

        const std::vector<Outport*>& connectedOutPorts = (*portIt)->getConnectedOutports();

        for (std::vector<Outport*>::const_iterator connectedPortIt = connectedOutPorts.begin(),
             connectedPortItEnd = connectedOutPorts.end();
             connectedPortIt!=connectedPortItEnd; ++connectedPortIt)
            if(*connectedPortIt)
                predecessors.insert((*connectedPortIt)->getProcessor());
    }

    return predecessors;
}

void ProcessorNetworkEvaluator::traversePredecessors(Processor* processor) {
    if (!hasBeenVisited(processor)) {
        processorsVisited_.push_back(processor);
        std::set<Processor*> directPredecessors = getDirectPredecessors(processor);

        for (std::set<Processor*>::iterator it = directPredecessors.begin(), itEnd=directPredecessors.end(); it!=itEnd; ++it)
            traversePredecessors(*it);

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
        std::set<Processor*> directPredecessors = getDirectPredecessors(processor, event);

        for (std::set<Processor*>::iterator it = directPredecessors.begin(), itEnd=directPredecessors.end(); it!=itEnd; ++it) {
            if ((*it)->hasInteractionHandler())
                (*it)->invokeInteractionEvent(event);

            propagateInteractionEvent(*it, event);
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

bool ProcessorNetworkEvaluator::isPortConnectedToProcessor(Port* port, Processor* processor) {
    bool isConnected = false;
    std::vector<PortConnection*> portConnections = processorNetwork_->getConnections();
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

    eventInitiator_ = 0;
}

bool ProcessorNetworkEvaluator::hasBeenVisited(Property* property) {
    if (std::find(propertiesVisited_.begin(), propertiesVisited_.end(), property)== propertiesVisited_.end())
        return false;

    return true;
}

void ProcessorNetworkEvaluator::onProcessorInvalidationEnd(Processor* p) {
    processorNetwork_->onProcessorInvalidationEnd(p);
    p->removeObserver(this);

    if (evaulationQueued_) {
        evaulationQueued_ = false;
        requestEvaluate();
    }
}

void ProcessorNetworkEvaluator::onProcessorNetworkEvaluateRequest() {
    requestEvaluate();
}

void ProcessorNetworkEvaluator::onProcessorNetworkUnlocked() {
    if (evaulationQueued_ || processorNetwork_->isModified()) {
        evaulationQueued_ = false;
        requestEvaluate();
    }
}

void ProcessorNetworkEvaluator::disableEvaluation() {
    evaluationDisabled_ = true;
}

void ProcessorNetworkEvaluator::enableEvaluation() {
    evaluationDisabled_ = false;

    if (evaulationQueued_) {
        evaulationQueued_ = false;
        requestEvaluate();
    }
}

ProcessorNetworkEvaluator* ProcessorNetworkEvaluator::getProcessorNetworkEvaluatorForProcessorNetwork(ProcessorNetwork* network) {
    std::map<ProcessorNetwork*,ProcessorNetworkEvaluator*>::iterator it = processorNetworkEvaluators_.find(network);

    if (it == processorNetworkEvaluators_.end())
        return new ProcessorNetworkEvaluator(network);

    return it->second;
}

void ProcessorNetworkEvaluator::requestEvaluate() {
    //evaluation has been triggered but not performed yet
    if (evaulationQueued_)
        return;

    //evaluation disabled
    if (processorNetwork_->islocked() || evaluationDisabled_) {
        evaulationQueued_ = true;
        return;
    }

    //wait for invalidation to finish before evaluating
    if (processorNetwork_->isInvalidating()) {
        evaulationQueued_ = true;
        return;
    }

    evaulationQueued_ = false;
    //if we haven't returned yet, perform evaluation of the network
    evaluate();
}


void ProcessorNetworkEvaluator::evaluate() {
    // lock processor network to avoid concurrent evaluation
    processorNetwork_->lock();

    // if the processor network has changed determine the new processor order
    if (processorNetwork_->isModified()) {
        activateDefaultRenderContext();
        initializeNetwork();
        determineProcessingOrder();
        processorNetwork_->setModified(false);
    }

    std::vector<Processor*>::iterator it;
    std::vector<Inport*>::const_iterator inport_it;
    activateDefaultRenderContext();

    for (it = processorsSorted_.begin(); it != processorsSorted_.end(); ++it) {
        if (!(*it)->isValid()) {
            if ((*it)->isReady()) {
                // re-initialize resources (e.g., shaders) if necessary
                if ((*it)->getInvalidationLevel() >= PropertyOwner::INVALID_RESOURCES)
                    (*it)->initializeResources();

                // call onChange for all invalid inports
                const std::vector<Inport*>& inports = (*it)->getInports();

                for (inport_it = inports.begin(); inport_it != inports.end(); ++inport_it)
                    (*inport_it)->callOnChangeIfChanged();

                // do the actual processing
                (*it)->process();

                // set processor as valid
                (*it)->setValid();
            }
            else{
                (*it)->doIfNotReady();
            }
        }
    }

    // unlock processor network to allow next evaluation
    processorNetwork_->unlock();
}

} // namespace
