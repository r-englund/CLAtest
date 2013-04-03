#include <inviwo/core/ports/port.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Port::Port(PortDirection direction, std::string identifier)
    : direction_(direction),
      identifier_(identifier)
{}

Port::~Port() {}

void Port::connectTo(Port* port) {
    if (std::find(connectedPorts_.begin(), connectedPorts_.end(), port) == connectedPorts_.end()) {
        connectedPorts_.push_back(port);
        port->connectTo(this);
    }
}

void Port::disconnectFrom(Port* port) {
    if (std::find(connectedPorts_.begin(), connectedPorts_.end(), port) != connectedPorts_.end()) {
        connectedPorts_.erase(std::remove(connectedPorts_.begin(), connectedPorts_.end(), port),
                                          connectedPorts_.end());
        port->disconnectFrom(this);
    }
}

void Port::invalidate() {
    if (direction_ == Port::INPORT) {
        if (processor_->isValid()) processor_->invalidate();
    } else if (direction_ == Port::OUTPORT) {
        for (size_t i=0; i<connectedPorts_.size(); i++)
            connectedPorts_[i]->invalidate();
    }
}

} // namespace
