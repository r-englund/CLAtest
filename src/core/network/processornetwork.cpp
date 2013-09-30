#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

ProcessorNetwork::ProcessorNetwork() : VoidObservable(),
    modified_(true),
	broadcastModification_(true),
    locked_(0) {}

ProcessorNetwork::~ProcessorNetwork() {}


void ProcessorNetwork::addProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)==processors_.end(),
              "Processor instance already contained in processor network.");
    processors_.push_back(processor);

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
            removeLink(processorLinks[i]->getDestinationProcessor(), processorLinks[i]->getSourceProcessor());

    // remove processor itself
    processors_.erase(std::remove(processors_.begin(), processors_.end(), processor), processors_.end());

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

bool ProcessorNetwork::isLinked(Processor* src, Processor* dst) {
    if (getProcessorLink(src, dst))
        return true;
    return false;
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
        if ((processorLinks_[i]->getSourceProcessor()==sourceProcessor &&
             processorLinks_[i]->getDestinationProcessor()==destProcessor) ||
            (processorLinks_[i]->getDestinationProcessor()==sourceProcessor &&
             processorLinks_[i]->getSourceProcessor()==destProcessor)) {
                //FIXME: unlink processors
                processorLinks_.erase(processorLinks_.begin()+i);
                break;
        }
    }
    modified();
}

void ProcessorNetwork::clear() {
    std::vector<Processor*> processors = processors_;   

    for (size_t i=0; i<processors.size(); i++)
        removeProcessor(processors[i]);

    std::vector<PortConnection*> connections = portConnections_;
    for (size_t i=0; i<connections.size(); i++)
        removeConnection(connections[i]->getOutport(), connections[i]->getInport());

    std::vector<ProcessorLink*> processorLinks = processorLinks_;
    for (size_t i=0; i<processorLinks_.size(); i++)
        removeLink(processorLinks[i]->getDestinationProcessor(), processorLinks[i]->getSourceProcessor() );
}

Processor* ProcessorNetwork::getProcessorByName(std::string identifier) const {
    for (size_t i=0; i<processors_.size(); i++)
        if (processors_[i]->getIdentifier()==identifier)
            return processors_[i];
    return 0;
}

ProcessorLink* ProcessorNetwork::getProcessorLink(Processor* processor1, Processor* processor2) const {
    for (size_t i=0; i<processorLinks_.size(); i++) { 
        if ((processorLinks_[i]->getSourceProcessor()==processor1 &&
             processorLinks_[i]->getDestinationProcessor()==processor2) ||
            (processorLinks_[i]->getDestinationProcessor()==processor1 &&
             processorLinks_[i]->getSourceProcessor()==processor2)) {
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
	}	
}

void ProcessorNetwork::serialize(IvwSerializer& s) const {
    s.serialize("Processors", processors_, "Processor");
    s.serialize("Connections", portConnections_, "Connection");
    s.serialize("ProcessorLinks", processorLinks_, "ProcessorLink");
}

void ProcessorNetwork::deserialize(IvwDeserializer& d) throw (Exception) {
    std::vector<PortConnection*> portConnections;
    std::vector<ProcessorLink*> processorLinks;

    //Processors
    try {
        d.deserialize("Processors", processors_, "Processor");
    }
    catch (const SerializationException& exception) {
        //Abort and clear all processors
        clear();
        throw AbortException("DeSerialization exception " + exception.getMessage());
    }
    catch (...) {
        //Remove all processors
        clear();
        throw AbortException("Unknown Exception.");        
    }

    //Connections
    try {
        d.deserialize("Connections", portConnections, "Connection");
        for (size_t i=0; i<portConnections.size(); i++) {
            Outport* outPort = portConnections[i]->getOutport();
            Inport* inPort = portConnections[i]->getInport();
            if (outPort && inPort) {
                addConnection(outPort, inPort);
            }
            else {
                LogWarn("Unable to establish port connection.");
            }
        }
    }
    catch (const SerializationException& exception) {             
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    }
    catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception."); 
    }

    //Links
    try {        
        d.deserialize("ProcessorLinks", processorLinks, "ProcessorLink");        
        for (size_t i=0; i<processorLinks.size(); i++) {
            Processor* inProcessor = processorLinks[i]->getSourceProcessor();
            Processor* outProcessor = processorLinks[i]->getDestinationProcessor();
            if (inProcessor && outProcessor) {
                std::vector<PropertyLink*> propertyLinks = processorLinks[i]->getPropertyLinks();
                for (size_t j=0; j<propertyLinks.size(); j++) {
                    Property* srcProperty = propertyLinks[j]->getSourceProperty();
                    Property* dstProperty = propertyLinks[j]->getDestinationProperty();
                    if (!( srcProperty && dstProperty)) {
                        processorLinks[i]->removePropertyLinks(srcProperty, dstProperty);
                        LogWarn("Unable to establish property link.");
                    }
                }
                
                if (processorLinks[i]->getPropertyLinks().size())
                    addLink(processorLinks[i]);
            }
            else {
                LogWarn("Unable to establish processor link.");
            }
        }
            
    }
    catch (const SerializationException& exception) {       
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    }
    catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception.");        
    }
	notifyObservers();
}

} // namespace
