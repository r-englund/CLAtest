#include <inviwo/core/ports/outport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Outport::Outport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : Port(identifier, invalidationLevel)
{}

Outport::~Outport() {}

bool Outport::isConnected() const { 
    return !(connectedInports_.empty()); 
}

bool Outport::isConnectedTo(Inport* port) const {
    return !(std::find(connectedInports_.begin(),connectedInports_.end(),port)==connectedInports_.end());
}

void Outport::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    for (size_t i=0; i<connectedInports_.size(); i++)
        connectedInports_[i]->invalidate(invalidationLevel);
}

void Outport::setInvalidationLevel( PropertyOwner::InvalidationLevel invalidationLevel )
{
    Port::setInvalidationLevel(invalidationLevel);
    for (size_t i=0; i<connectedInports_.size(); i++)
        connectedInports_[i]->setInvalidationLevel(invalidationLevel);
}

template <typename T>
void Outport::getSuccessorsUsingPortType(std::vector<Processor*>& successorProcessors) {
    for (size_t i=0; i<connectedInports_.size(); i++) {
        Processor* decendantProcessor = connectedInports_[i]->getProcessor();

        if (std::find(successorProcessors.begin(), successorProcessors.end(), decendantProcessor)== successorProcessors.end())
            successorProcessors.push_back(connectedInports_[i]->getProcessor());

        std::vector<Outport*> outports = decendantProcessor->getOutports();
        for (size_t j=0; j<outports.size(); j++) {
            T* outPort = dynamic_cast<T*>(outports[j]);
            if (outPort)
                outPort->template getSuccessorsUsingPortType<T>(successorProcessors);
        }
    }
}

std::vector<Processor*> Outport::getDirectSuccessors() {
    std::vector<Processor*> successorProcessors;
    getSuccessorsUsingPortType<Outport>(successorProcessors);
    return successorProcessors;
}

//Is called exclusively by Inport, which means a connection has been made.
void Outport::connectTo(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) == connectedInports_.end()) {
        connectedInports_.push_back(inport);
        Port::invalidate(invalidationLevel_);
    }
}

//Is called exclusively by Inport, which means a connection has been removed.
void Outport::disconnectFrom(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) != connectedInports_.end()) {
        connectedInports_.erase(std::remove(connectedInports_.begin(), connectedInports_.end(), inport),
            connectedInports_.end());
        Port::invalidate(invalidationLevel_);
    }
}


} // namespace
