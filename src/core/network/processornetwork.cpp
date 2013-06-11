#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

ProcessorNetwork::ProcessorNetwork() : VoidObservable(),
    modified_(true),
	broadcastModification_(false) {}

ProcessorNetwork::~ProcessorNetwork() {}


void ProcessorNetwork::addProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)==processors_.end(),
              "Processor instance already contained in processor network.");
    processors_.push_back(processor);
	setBroadcastModification(true);
    modified();
}

void ProcessorNetwork::removeProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)!=processors_.end(),
              "Processor instance not contained in processor network.");

    // remove all connections for this processor
    std::vector<PortConnection*> portConnections = portConnections_;
    for (size_t i=0; i<portConnections.size(); i++)
        if (portConnections[i]->involvesProcessor(processor))
            removeConnection(portConnections[i]->getOutport(), portConnections[i]->getInport());

    // remove all links for this processor
    std::vector<ProcessorLink*> processorLinks = processorLinks_;
    for (size_t i=0; i<processorLinks.size(); i++)
        if (processorLinks[i]->involvesProcessor(processor))
            removeLink(processorLinks[i]->getOutProcessor(), processorLinks[i]->getInProcessor());

    // remove processor itself
    processors_.erase(std::remove(processors_.begin(), processors_.end(), processor), processors_.end());

	setBroadcastModification(true);
    modified();
}


void ProcessorNetwork::addConnection(Outport* sourcePort, Inport* destPort) {
    ivwAssert(!sourcePort->isConnectedTo(destPort), "Ports already connected.");
    destPort->connectTo(sourcePort);
    portConnections_.push_back(new PortConnection(sourcePort, destPort));
    modified();
}

void ProcessorNetwork::removeConnection(Outport* sourcePort, Inport* destPort) {
    for (size_t i=0; i<portConnections_.size(); i++) {
        if (portConnections_[i]->getOutport()==sourcePort && portConnections_[i]->getInport()==destPort) {                
            destPort->disconnectFrom(sourcePort);
            delete portConnections_[i];
            portConnections_.erase(portConnections_.begin()+i);
            break;
        }
    }
    modified();
}

void ProcessorNetwork::addLink(ProcessorLink* processorLink) {        
    processorLinks_.push_back(processorLink);
    modified();
}

void ProcessorNetwork::addLink(Processor* sourceProcessor, Processor* destProcessor) {        
    addLink(new ProcessorLink(sourceProcessor, destProcessor));
}

void ProcessorNetwork::removeLink(Processor* sourceProcessor, Processor* destProcessor) {
    for (size_t i=0; i<processorLinks_.size(); i++) {            
        if ((processorLinks_[i]->getInProcessor()==sourceProcessor &&
             processorLinks_[i]->getOutProcessor()==destProcessor) ||
            (processorLinks_[i]->getOutProcessor()==sourceProcessor &&
             processorLinks_[i]->getInProcessor()==destProcessor)) {
                //FIXME: unlink processors
                processorLinks_.erase(processorLinks_.begin()+i);
                break;
        }
    }
    modified();
}


Processor* ProcessorNetwork::getProcessorByName(std::string identifier) const {
    for (size_t i=0; i<processors_.size(); i++)
        if (processors_[i]->getIdentifier()==identifier)
            return processors_[i];
    return 0;
}

ProcessorLink* ProcessorNetwork::getProcessorLink(Processor* processor1, Processor* processor2) const {
    for (size_t i=0; i<processorLinks_.size(); i++) { 
        if ((processorLinks_[i]->getInProcessor()==processor1 &&
             processorLinks_[i]->getOutProcessor()==processor2) ||
            (processorLinks_[i]->getOutProcessor()==processor1 &&
             processorLinks_[i]->getInProcessor()==processor2)) {
            return processorLinks_[i];
        }
    }
    LogWarn("No link between "+processor1->getIdentifier()+" and "+
                               processor2->getIdentifier()+" contained in processor network.");
    return 0;
}

inline void ProcessorNetwork::modified() {
	modified_ = true; 
	if (broadcastModification_) {
		notifyObservers();
		setBroadcastModification(false);
	}
}

void ProcessorNetwork::serialize(IvwSerializer& s) const {
    s.serialize("Processors", processors_, "Processor");
    s.serialize("Connections", portConnections_, "Connection");
    s.serialize("ProcessorLinks", processorLinks_, "ProcessorLink");
}

void ProcessorNetwork::deserialize(IvwDeserializer& d) {
    std::vector<PortConnection*> portConnections;
    std::vector<ProcessorLink*> processorLinks;

    d.deserialize("Processors", processors_, "Processor");
    d.deserialize("Connections", portConnections, "Connection");
    d.deserialize("ProcessorLinks", processorLinks, "ProcessorLink");

    for (size_t i=0; i<portConnections.size(); i++)
        addConnection(portConnections[i]->getOutport(), portConnections[i]->getInport());

    for (size_t i=0; i<processorLinks.size(); i++)
        addLink(processorLinks[i]);
}

} // namespace
