#include <inviwo/core/ports/outport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Outport::Outport(std::string identifier)
: identifier_(identifier)
{}

Outport::~Outport() {}

void Outport::connectTo(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) == connectedInports_.end()) {
        connectedInports_.push_back(inport);
        inport->invalidate();
        inport->connectTo(this);
    }
}

void Outport::disconnectFrom(Inport* inport) {
    if (std::find(connectedInports_.begin(), connectedInports_.end(), inport) != connectedInports_.end()) {
        connectedInports_.erase(std::remove(connectedInports_.begin(), connectedInports_.end(), inport),
            connectedInports_.end());
        inport->invalidate();
        inport->disconnectFrom(this);
    }
}

void Outport::invalidate() {
    for (size_t i=0; i<connectedInports_.size(); i++)
        connectedInports_[i]->invalidate();
}

} // namespace
