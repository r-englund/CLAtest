#include <inviwo/core/processors/processor.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/util/inviwofactorybase.h>

namespace inviwo {

ProcessorClassName(Processor, "Processor"); 
ProcessorCategory(Processor, "undefined");
ProcessorCodeState(Processor, CODE_STATE_EXPERIMENTAL); 

Processor::Processor() : VoidObservable(),
    initialized_(false), processorWidget_(0), identifier_("undefined"),
    showProgressBar_(false), progress_(0.0f), beginLoopProgress_(-1.0f)
{}

Processor::~Processor() {}

void Processor::addPort(Inport* port, std::string portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    inports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
}

void Processor::addPort(Inport& port, std::string portDependencySet) {
    addPort(&port, portDependencySet);
}

void Processor::addPort(Outport* port, std::string portDependencySet) {
    // TODO: check if port with same name has been added before
    port->setProcessor(this);
    outports_.push_back(port);
    portDependencySets_.insert(portDependencySet, port);
}

void Processor::addPort(Outport& port, std::string portDependencySet) {
    addPort(&port, portDependencySet);
}

Port* Processor::getPort(std::string identifier) const {
    std::vector<Port*> ports;
    ports.insert(ports.begin(), inports_.begin(), inports_.end());
    ports.insert(ports.end(), outports_.begin(), outports_.end());
    for (size_t i=0; i<ports.size(); i++)
        if (ports[i]->getIdentifier() == identifier)
            return ports[i];
    return NULL;
}

std::vector<Port*> Processor::getPortsByDependencySet(std::string portDependencySet) {
     return portDependencySets_.getGroupedData(portDependencySet);
}

std::vector<std::string> Processor::getPortDependencySets() {
    return portDependencySets_.getGroupKeys();
}

std::string Processor::getPortDependencySet(Port* port) {
    return portDependencySets_.getKey(port);

}

bool Processor::allInportsConnected() const {
    for (size_t i=0; i<inports_.size(); i++)
        if (!inports_[i]->isConnected())
            return false;
    return true;
}

void Processor::initialize() {
    for (size_t i=0; i<inports_.size(); i++)
        inports_[i]->initialize();
    for (size_t i=0; i<outports_.size(); i++)
        outports_[i]->initialize();
    initialized_ = true;
}

void Processor::deinitialize() {
    for (size_t i=0; i<inports_.size(); i++)
        inports_[i]->deinitialize();
    for (size_t i=0; i<outports_.size(); i++)
        outports_[i]->deinitialize();

    portDependencySets_.deinitialize();
    initialized_ = false;
}

void Processor::invalidate(PropertyOwner::InvalidationLevel invalidationLevel) {
    PropertyOwner::invalidate(invalidationLevel);
    for (size_t i=0; i<outports_.size(); i++)
        outports_[i]->invalidate(PropertyOwner::INVALID_OUTPUT);
    notifyObservers();
}

void Processor::process() {
}


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


void Processor::updateProgress(float progress) {
    if (showProgressBar_) {
        ivwAssert(progress>=progress_, "Progress should always increase");
        ivwAssert(progress>=0.0f&&progress<=1.0, "Progress out of bounds.");
        progress_ = progress;
        notifyObservers();
    }
}

void Processor::updateProgressLoop(size_t loopVar, size_t maxLoopVar, float endLoopProgress) {
    if (showProgressBar_) {
        if (beginLoopProgress_<=0.0f)
            beginLoopProgress_ = progress_;
        float normalizedLoopVar = static_cast<float>(loopVar)/static_cast<float>(maxLoopVar);
        progress_ = beginLoopProgress_+normalizedLoopVar*(endLoopProgress-beginLoopProgress_);
        if (loopVar == maxLoopVar)
            beginLoopProgress_ = -1.0f;
        notifyObservers();
    }
}



MetaData* Processor::getMetaData(std::string className) {
    MetaData* meta = 0;
    for (size_t i=0; i<metaData_.size(); i++) {
        if (metaData_[i]->getClassName()==className) {
            meta = metaData_[i];
            break;
        }
    }

    if (!meta) {
        meta = dynamic_cast<MetaData*>(MetaDataFactory::getRef().create(className));
        metaData_.push_back(meta);
    }
    return meta;
}

void Processor::serialize(IvwSerializer& s) const {
    s.serialize("type", getClassName(), true);
    s.serialize("identifier", identifier_, true);
    s.serialize("MetaDataList", metaData_, "MetaData") ;

	if (interactionHandlers_.size() != 0) 
		s.serialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");

    PropertyOwner::serialize(s);


}

void Processor::deserialize(IvwDeserializer& d) {
    std::string className;
    d.deserialize("type", className, true);
    d.deserialize("identifier", identifier_, true);
    d.deserialize("MetaDataList", metaData_, "MetaData") ;

	if (interactionHandlers_.size() != 0)
		d.deserialize("InteractonHandlers", interactionHandlers_, "InteractionHandler");

    PropertyOwner::deserialize(d);


}

} // namespace
