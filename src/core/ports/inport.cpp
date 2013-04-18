#include <inviwo/core/ports/inport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Inport::Inport(std::string identifier)
: identifier_(identifier)
{}

Inport::~Inport() {}

void Inport::connectTo(Outport* outport) {
    connectedOutport_ = outport;
    invalidate();
    outport->connectTo(this);
}

void Inport::disconnectFrom(Outport* outport) {
    // TODO: check if ports are connected
    ivwAssert(connectedOutport_==outport, "Ports are not connected.");
    connectedOutport_ = 0;
    invalidate();
    outport->disconnectFrom(this);
}

void Inport::invalidate() {
    //if (processor_->isValid())
        processor_->invalidate();
}

} // namespace
