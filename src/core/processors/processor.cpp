#include "inviwo/core/processors/processor.h"

namespace inviwo {

    Processor::Processor()
        : identifier_("undefined"),
          processorWidget_(0)
    {}
    Processor::~Processor() {}

    Processor* Processor::create() const {
        return new Processor();
    }

    void Processor::addPort(Port* port) {
        // TODO: check if port with same name has been added before
        port->setProcessor(this);
        if (port->isOutport()) outports_.push_back(port);
        else inports_.push_back(port);
    }

    void Processor::addPort(Port& port) {
        addPort(&port);
    }

    Port* Processor::getPort(std::string identifier) const {
        std::vector<Port*> ports;
        ports.insert(ports.begin(), inports_.begin(), inports_.end());
        ports.insert(ports.end(), outports_.begin(), outports_.end());
        for (size_t i=0; i<ports.size(); i++)
            if (ports[i]->getIdentifier() == identifier)
                return ports[i];
        return 0;
    }

    void Processor::initialize() throw (Exception) {
        for (size_t i=0; i<inports_.size(); i++)
            inports_[i]->initialize();
        for (size_t i=0; i<outports_.size(); i++)
            outports_[i]->initialize();
    }

    void Processor::deinitialize() throw (Exception) {
        for (size_t i=0; i<inports_.size(); i++)
            inports_[i]->deinitialize();
        for (size_t i=0; i<outports_.size(); i++)
            outports_[i]->deinitialize();
    }

    void Processor::invalidate() {
        PropertyOwner::invalidate();
        for (size_t i=0; i<outports_.size(); i++)
            outports_[i]->invalidate();
    }

    void Processor::beforeProcess() {}
    void Processor::process() {}
    void Processor::afterProcess() {}


    void Processor::addInteractionHandler(InteractionHandler* interactionHandler) {
        interactionHandlers_.push_back(interactionHandler);
    }

    void Processor::removeInteractionHandler(InteractionHandler* interactionHandler) {
        for (size_t i=0; i<interactionHandlers_.size(); i++)
            if (interactionHandlers_[i] == interactionHandler) {
                interactionHandlers_.erase(interactionHandlers_.begin()+i);
                i = interactionHandlers_.size();
            }
    }

    void Processor::invokeInteractionEvent(Event* event) {
        for (size_t i=0; i<interactionHandlers_.size(); i++)
            interactionHandlers_[i]->invokeEvent(event);
    }

    void Processor::serialize(XmlSerializer& /*s*/) const {}
    void Processor::deserialize(XmlDeserializer& /*d*/) {}

} // namespace
