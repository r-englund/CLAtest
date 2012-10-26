#include "inviwo/core/network/processornetworkevaluator.h"
#include "inviwo/core/processors/canvasprocessor.h"

namespace inviwo {

const std::string ProcessorNetworkEvaluator::logSource_ = "ProcessorNetworkEvaluator";

ProcessorNetworkEvaluator::ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork)
    : processorNetwork_(processorNetwork) { 
    registeredCanvases_.clear();
    initializeNetwork();
    renderContext_ = 0;
}

ProcessorNetworkEvaluator::~ProcessorNetworkEvaluator() {}

void ProcessorNetworkEvaluator::setProcessorNetwork(ProcessorNetwork* processorNetwork) {
    processorNetwork_ = processorNetwork;
    initializeNetwork();
}

void ProcessorNetworkEvaluator::initializeNetwork() {
    ivwAssert(processorNetwork_, "processorNetwork_ not initialized, call setProcessorNetwork()");
    // initialize network
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++)
        processors[i]->initialize();
}

void ProcessorNetworkEvaluator::registerCanvas(Canvas* canvas, std::string associatedProcessName) {
    canvas->setNetworkEvaluator(this);
    registeredCanvases_.push_back(canvas);
    std::vector<CanvasProcessor*> canvasProcessors = processorNetwork_->getProcessorsByType<CanvasProcessor>();
    for (size_t i=0; i<canvasProcessors.size(); i++) {
        if(canvasProcessors[i]->getIdentifier() == associatedProcessName) {
            canvasProcessors[i]->setCanvas(canvas);
        }
    }
}

void ProcessorNetworkEvaluator::deRegisterCanvas(Canvas *canvas) {

    /*
    CanvasProcessor* canvasProc = dynamic_cast<CanvasProcessor*>(processorNetwork_->getProcessorByName(associatedProcessName));

    if(canvasProc) {

        Canvas* canvas = canvasProc->getCanvas();

       if(canvas) {

            if( std::find(registeredCanvases_.begin(), registeredCanvases_.end(), canvas)== registeredCanvases_.end()) {
                return;
            }

             canvas->setNetworkEvaluator(0);
            canvasProc->setCanvas(0);

            registeredCanvases_.erase(std::remove(registeredCanvases_.begin(), registeredCanvases_.end(),
                                        canvas), registeredCanvases_.end());
        }
    }
    */

    registeredCanvases_.erase(std::remove(registeredCanvases_.begin(), registeredCanvases_.end(),
                                        canvas), registeredCanvases_.end());
}


bool ProcessorNetworkEvaluator::hasBeenVisited(Processor* processor) {
    for (size_t i=0; i<processorsVisited_.size(); i++)
        if (processorsVisited_[i] == processor)
            return true;
    return false;
}

std::vector<Processor*> ProcessorNetworkEvaluator::getDirectPredecessors(Processor* processor) {
    std::vector<Processor*> predecessors;
    std::vector<Port*> inports = processor->getInports();
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Port* connectedOutport = portConnections[j]->getOutport();
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

void ProcessorNetworkEvaluator::sortTopologically() {
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

void ProcessorNetworkEvaluator::propagateMouseEvent(Processor* processor, MouseEvent* mouseEvent) {
    if (!hasBeenVisited(processor)) {
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++) {
            if (directPredecessors[i]->hasInteractionHandler())
                directPredecessors[i]->invokeInteractionEvent(mouseEvent);
            // TODO: transform positions based on subcanvas arrangement
            propagateMouseEvent(directPredecessors[i], mouseEvent);
        }
    }
}

void ProcessorNetworkEvaluator::propagateMouseEvent(Canvas* canvas, MouseEvent* mouseEvent) {
    // find the canvas processor from which the event was emitted
    Processor* eventInitiator=0; 
    std::vector<Processor*> processors = processorNetwork_->getProcessors();
    for (size_t i=0; i<processors.size(); i++) {
        if ((dynamic_cast<CanvasProcessor*>(processors[i])) &&
            (dynamic_cast<CanvasProcessor*>(processors[i])->getCanvas()==canvas)) {
                eventInitiator = processors[i];
                i = processors.size();
        }
    }

    if(!eventInitiator) return;
    processorsVisited_.clear();
    propagateMouseEvent(eventInitiator, mouseEvent);
}

void ProcessorNetworkEvaluator::evaluate() {
    repaintRequired_ = false;
    
    // TODO: perform only if network has been changed
    sortTopologically();

    renderContext_->switchContext();
    for (size_t i=0; i<processorsSorted_.size(); i++) {
        if (!processorsSorted_[i]->isValid()) {
            if (processorsSorted_[i]->allInportsConnected()) {
                processorsSorted_[i]->process();
                if (!dynamic_cast<CanvasProcessor*>(processorsSorted_[i])) {
                    processorsSorted_[i]->setValid();
                    renderContext_->switchContext();
                }
                repaintRequired_ = true;
            }
        }
    }
    if (repaintRequired_)
        for (size_t i=0; i<registeredCanvases_.size(); i++)
            registeredCanvases_[i]->repaint();
}

} // namespace
