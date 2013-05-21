#include <inviwo/core/ports/inport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Inport::Inport(std::string identifier)
: Port(identifier), connectedOutport_(NULL)
{}

Inport::~Inport() {}

//Inport should determine if we can connect to the outport
void Inport::connectTo(Outport* outport) {
    connectedOutport_ = outport;
    outport->connectTo(this);
    invalidate();
}

void Inport::disconnectFrom(Outport* outport) {
    // TODO: check if ports are connected
    ivwAssert(connectedOutport_==outport, "Ports are not connected.");
    connectedOutport_ = NULL;
    outport->disconnectFrom(this);
    invalidate();
}

bool Inport::isConnected() const { 
    return (connectedOutport_!=NULL); 
}

bool Inport::isConnectedTo(Outport* outport) const {
    return connectedOutport_==outport;
}

void Inport::invalidate() {
    Port::invalidate();
}

template <typename T>
void Inport::getPredecessorsUsingPortType(std::vector<Processor*>& predecessorsProcessors) {
    Port* connectedOutport = dynamic_cast<Port*>(connectedOutport_);
    if (connectedOutport) {
        Processor* predecessorsProcessor = connectedOutport->getProcessor();

        if (std::find(predecessorsProcessors.begin(), predecessorsProcessors.end(), predecessorsProcessor)== predecessorsProcessors.end())
            predecessorsProcessors.push_back(predecessorsProcessor);

        std::vector<Inport*> inports = predecessorsProcessor->getInports();
        for (size_t j=0; j<inports.size(); j++) {
            T* inPort = dynamic_cast<T*>(inports[j]);
            if (inPort)
                inPort->getPredecessorsUsingPortType<T>(predecessorsProcessors);            
        }
    }
}

std::vector<Processor*> Inport::getPredecessors() {
    std::vector<Processor*> predecessorsProcessors;
    getPredecessorsUsingPortType<Inport>(predecessorsProcessors);
    return predecessorsProcessors;
}

} // namespace
