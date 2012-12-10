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
    linkEvaluator_ = new LinkEvaluator();
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
            //directPredecessors[i]->invalidate();  //TODO: Check if this is needed
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
    //eventInitiator->invalidate(); //TODO: Check if this is needed
}

bool ProcessorNetworkEvaluator::isPortConnectedToProcessor(Port* port, Processor *processor) {
    bool isConnected = false;
    std::vector<PortConnection*> portConnections = processorNetwork_->getPortConnections();

    std::vector<Port*> outports = processor->getOutports();   
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

    std::vector<Port*> inports = processor->getInports();   
    for (size_t i=0; i<inports.size(); i++) {
        for (size_t j=0; j<portConnections.size(); j++) {
            const Port* curInport = portConnections[j]->getInport();
            if (curInport == inports[i]) {
                const Port* connectedOutport = portConnections[j]->getOutport();
                if (connectedOutport == port) {
                    isConnected = true;
                    break;
                }
            }
        }
    }

    return isConnected;

}

void ProcessorNetworkEvaluator::propagateResizeEvent(Processor* processor, ResizeEvent* resizeEvent) {
    if (!hasBeenVisited(processor)) {
        processor->invalidate();
        processorsVisited_.push_back(processor);
        std::vector<Processor*> directPredecessors = getDirectPredecessors(processor);
        for (size_t i=0; i<directPredecessors.size(); i++) {
            bool invalidate=false;
            
            if (directPredecessors[i]->hasInteractionHandler())
                directPredecessors[i]->invokeInteractionEvent(resizeEvent);
            
            std::vector<Port*> outports = directPredecessors[i]->getOutports();
            for (size_t j=0; j<outports.size(); j++) {
                ImagePort* imagePort = dynamic_cast<ImagePort*>(outports[j]);
                if (imagePort) {
                    if (isPortConnectedToProcessor(imagePort, processor)) {
                        imagePort->resize(resizeEvent->canvasSize());
                        invalidate = true;
                    }
                }
            }
            
            std::vector<string> portGroups = directPredecessors[i]->getPortGroupNames();
            std::vector<Port*> ports;
            for (size_t j=0; j<portGroups.size(); j++) {
                ports.clear();
                ports = directPredecessors[i]->getPortsByGroup(portGroups[j]);

                ivec2 dimMax(0);
                for (size_t j=0; j<ports.size(); j++) {
                    ImagePort* imagePort = dynamic_cast<ImagePort*>(ports[j]);
                    if (imagePort && imagePort->isOutport()) {
                        ivec2 dim = imagePort->getDimensions();
                        //TODO: Determine max dimension based on aspect ratio?
                        if ((dimMax.x<dim.x) || (dimMax.y<dim.y)) {
                            dimMax = imagePort->getDimensions();
                        }
                    }
                }

                if(dimMax.x>0 && dimMax.y>0) {
                    for (size_t j=0; j<ports.size(); j++) {
                        ImagePort* imagePort = dynamic_cast<ImagePort*>(ports[j]);
                        if (imagePort && imagePort->isInport()) {
                            imagePort->resize(dimMax);
                        }
                    }
                }                
                
            }
            if (invalidate) directPredecessors[i]->invalidate();
            propagateResizeEvent(directPredecessors[i], resizeEvent);
        }
    }
}

void ProcessorNetworkEvaluator::propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent) {
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
    propagateResizeEvent(eventInitiator, resizeEvent);

    bool invalidate=false;
    std::vector<Port*> inports = eventInitiator->getInports();
    for (size_t j=0; j<inports.size(); j++) {
        ImagePort* imagePort = dynamic_cast<ImagePort*>(inports[j]);
        if (imagePort) {
            imagePort->resize(resizeEvent->canvasSize());
            invalidate = true;
        }
    }
    if (invalidate) eventInitiator->invalidate();
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

void ProcessorNetworkEvaluator::getConnectedLinksToDestinationProperty(PropertyLink* propertyLink) {
    Property* destinationProperty = propertyLink->getDestinationProperty();
    if( std::find(destinationPropertiesVisited_.begin(), destinationPropertiesVisited_.end(), destinationProperty)== destinationPropertiesVisited_.end()) {
        destinationPropertiesVisited_.push_back(destinationProperty);
    }
    else {
        if (propertyLink->isBidirectional()) {
            destinationProperty = propertyLink->getSourceProperty();
            if( std::find(destinationPropertiesVisited_.begin(), destinationPropertiesVisited_.end(), destinationProperty)== destinationPropertiesVisited_.end())
                destinationPropertiesVisited_.push_back(destinationProperty);
            else
                return;
        }
        else
            return;
    }

    std::vector<PropertyLink*> propertyLinks = getConnectedPropertyLinks(destinationProperty);

    for (size_t i=0; i<propertyLinks.size(); i++) {
        if (propertyLinks[i]==propertyLink) continue;
        if (destinationProperty == propertyLinks[i]->getSourceProperty())
            linkEvaluator_->evaluate(propertyLinks[i]->getSourceProperty(), propertyLinks[i]->getDestinationProperty());
        else if (propertyLinks[i]->isBidirectional())
            linkEvaluator_->evaluate(propertyLinks[i]->getDestinationProperty(), propertyLinks[i]->getSourceProperty());

        getConnectedLinksToDestinationProperty(propertyLinks[i]);  
    }    
}

void ProcessorNetworkEvaluator::executePropertyLinking(Property* sourceProperty) {
    
    std::vector<PropertyLink*> propertyLinks = getConnectedPropertyLinks(sourceProperty);

    //source is considered as visited destination
    destinationPropertiesVisited_.clear();
    destinationPropertiesVisited_.push_back(sourceProperty);

    for (size_t i=0; i<propertyLinks.size(); i++) {
        Property* destinationProperty=0;
        if (propertyLinks[i]->getSourceProperty() == sourceProperty) {
            destinationProperty = propertyLinks[i]->getDestinationProperty();
        }
        else if (propertyLinks[i]->isBidirectional()) {
            destinationProperty = propertyLinks[i]->getSourceProperty();
        }

        linkEvaluator_->evaluate(sourceProperty, destinationProperty);
        getConnectedLinksToDestinationProperty(propertyLinks[i]);
    }
}

void ProcessorNetworkEvaluator::evaluate() {

   //TODO: This is just for testing. Remove it once we have proper linking interface
   //No cyclic check or change propagation
    std::vector<ProcessorLink*> links = processorNetwork_->getProcessorLinks();
    for (size_t i=0; i<links.size(); i++) {
        if (!links[i]->isValid()) {
            links[i]->evaluate();
        }
    }

    repaintRequired_ = false;
    
    // TODO: perform only if network has been changed
    sortTopologically();

    renderContext_->switchContext();
    for (size_t i=0; i<processorsSorted_.size(); i++) {
        if (!processorsSorted_[i]->isValid()) {
            if (processorsSorted_[i]->allInportsConnected()) {
                processorsSorted_[i]->process();                
                processorsSorted_[i]->setValid();
                if (!dynamic_cast<CanvasProcessor*>(processorsSorted_[i])) {
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
