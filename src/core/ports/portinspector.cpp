#include <inviwo/core/ports/portinspector.h>
#include <inviwo/core/io/serialization/ivwserialization.h>

namespace inviwo {
PortInspector::PortInspector(std::string portClassName, std::string inspectorWorkspace)
    : inspectorNetworkFileName_(inspectorWorkspace)
    , portClassName_(portClassName)
    , active_(false)
    , inspectorNetwork_(NULL) {
}

PortInspector::~PortInspector() {
    if(inspectorNetwork_) {
        delete inspectorNetwork_;
    }
}

void PortInspector::setActive(bool val) {
    active_ = val;
}

bool PortInspector::isActive() {
    return active_;
}

std::string PortInspector::getInspectorNetworkFileName() {
    return inspectorNetworkFileName_;
}

std::string PortInspector::getPortClassName() {
    return portClassName_;
}

std::vector<Inport*> PortInspector::getInports() {
    if(!inspectorNetwork_) {
        initialize();
    }
    return inPorts_;
}

CanvasProcessor* PortInspector::getCanvasProcessor() {
    if(!inspectorNetwork_) {
        initialize();
    }
    return canvasProcessor_;
}
    
std::vector<PortConnection*>  PortInspector::getConnections(){
    if(!inspectorNetwork_) {
        initialize();
    }
    return connections_;
}
std::vector<ProcessorLink*> PortInspector::getProcessorLinks() {
    if(!inspectorNetwork_) {
        initialize();
    }
    return processorLinks_;
}
std::vector<Processor*> PortInspector::getProcessors(){
    if(!inspectorNetwork_) {
        initialize();
    }
    return processors_;
}
    
void PortInspector::initialize() {

    // Deserialize the network
    IvwDeserializer xmlDeserializer(inspectorNetworkFileName_);
    inspectorNetwork_ = new ProcessorNetwork();
    inspectorNetwork_->deserialize(xmlDeserializer);

    processors_ = inspectorNetwork_->getProcessors();

    for(size_t i = 0; i < processors_.size(); i++) {
        Processor* processor = processors_[i];
        
        // Set Identifiers
        std::string newIdentifier = getPortClassName()+"_Port_Inspector_"+processor->getIdentifier();
        processor->setIdentifier(newIdentifier);
        processor->initialize();
        
        // Find the and save inports.
        std::vector<Inport*> inports = processor->getInports();
        for(size_t i = 0; i < inports.size(); i++) {
            if(!inports[i]->isConnected()) {
                inPorts_.push_back(inports[i]);
            }
        }
        
        // Find and save the canvasProcessor
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>(processor);
        if (canvasProcessor) {
            canvasProcessor_ = canvasProcessor;
        }
    }
    
    // Store the connections and and disconnect them.
    connections_ = inspectorNetwork_->getPortConnections();
    for (size_t i=0; i<connections_.size(); i++) {
        connections_[i]->getInport()->disconnectFrom(connections_[i]->getOutport());
    }

    // store the processor links.
    processorLinks_ = inspectorNetwork_->getProcessorLinks();
}





} // namespace

