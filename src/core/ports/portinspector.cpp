#include <inviwo/core/ports/portinspector.h>
#include <inviwo/core/io/serialization/ivwserialization.h>

namespace inviwo {
PortInspector::PortInspector(std::string portClassName, std::string inspectorWorkspace)
    : portClassName_(portClassName)
    , inspectorNetworkFileName_(inspectorWorkspace)
    , inspectorNetwork_(NULL) {
}

PortInspector::~PortInspector() {
    if(inspectorNetwork_) {
        delete inspectorNetwork_;
    }
}

std::string PortInspector::getInspectorWorkspace() {
    return inspectorNetworkFileName_;
}

std::string PortInspector::getPortClassName() {
    return portClassName_;
}

ProcessorNetwork* PortInspector::getInspectorNetwork() {
    if(!inspectorNetwork_) {
        initialize();
    }
    return inspectorNetwork_;
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

void PortInspector::initialize() {
    IvwDeserializer xmlDeserializer(inspectorNetworkFileName_);
    ProcessorNetwork* inspectorNetwork_ = new ProcessorNetwork();
    inspectorNetwork_->deserialize(xmlDeserializer);


    std::vector<Processor*> processors = inspectorNetwork_->getProcessors();
    for(size_t i = 0; i < processors.size(); i++) {
        Processor* processor = processors[i];
        std::vector<Inport*> inports = processor->getInports();
        for(size_t i = 0; i < inports.size(); i++) {
            if(!inports[i]->isConnected()) {
                inPorts_.push_back(inports[i]);
            }
        }
    }
}

} // namespace

