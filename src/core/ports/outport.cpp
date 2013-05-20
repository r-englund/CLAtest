#include <inviwo/core/ports/outport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Outport::Outport(std::string identifier)
: Port(identifier)
{}

Outport::~Outport() {}

bool Outport::isConnected() const { 
    return !(connectedInports_.empty()); 
}

bool Outport::isConnectedTo(Inport* port) const {
    return !(std::find(connectedInports_.begin(),connectedInports_.end(),port)==connectedInports_.end());
}

void Outport::invalidate() {
    for (size_t i=0; i<connectedInports_.size(); i++)
        connectedInports_[i]->invalidate();
}

template <typename T>
void Outport::getDescendantProcessorsUsingPortType(std::vector<Processor*>& descendantProcessors) {
    for (size_t i=0; i<connectedInports_.size(); i++) {
        Processor* decendantProcessor = connectedInports_[i]->getProcessor();

        if (std::find(descendantProcessors.begin(), descendantProcessors.end(), decendantProcessor)== descendantProcessors.end())
            descendantProcessors.push_back(connectedInports_[i]->getProcessor());

        std::vector<Outport*> outports = decendantProcessor->getOutports();
        for (size_t j=0; j<outports.size(); j++) {
            T* outPort = dynamic_cast<T*>(outports[j]);
            if (outPort)
                outPort->getDescendantProcessorsUsingPortType<T>(descendantProcessors);            
        }
    }
}

std::vector<Processor*> Outport::getDescendantProcessors() {
    std::vector<Processor*> descendantProcessors;
    getDescendantProcessorsUsingPortType<Outport>(descendantProcessors);
    return descendantProcessors;
}

//Is called exclusively by Inport, which means a connection has been made.
void Outport::connectTo(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) == connectedInports_.end()) {
        connectedInports_.push_back(inport);
    }
}

//Is called exclusively by Inport, which means a connection has been removed.
void Outport::disconnectFrom(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) != connectedInports_.end()) {
        connectedInports_.erase(std::remove(connectedInports_.begin(), connectedInports_.end(), inport),
            connectedInports_.end());
    }
}

} // namespace
