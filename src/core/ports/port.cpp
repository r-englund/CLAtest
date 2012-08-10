#include "inviwo/core/ports/port.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

    Port::Port(PortDirection direction, std::string identifier)
        : direction_(direction),
          identifier_(identifier)
    {}

    Port::~Port() {}

    void Port::initialize() {}
    void Port::deinitialize() {}

    void Port::connectTo(Port* port) {
        connected_ = true;
        connectedPorts_.push_back(port);
        port->connectedPorts_.push_back(this);
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
