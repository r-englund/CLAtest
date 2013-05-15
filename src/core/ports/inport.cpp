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

} // namespace
