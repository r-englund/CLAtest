#include <inviwo/core/network/processornetwork.h>

//TODO: remove these includes
#include <modules/base/processors/firstivwprocessor.h>
#include <modules/base/processors/imagesource.h>
#include <modules/base/processors/entryexitpoints.h>
#include <modules/base/processors/simpleraycaster.h>
#include <modules/base/processors/imagesource.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <inviwo/core/processors/datasource/volumesource.h>

namespace inviwo {

// FIXME: temporary method supporting non serialization-based network creation
void ProcessorNetwork::connectPorts(Port* sourcePort, Port* destPort) {
    destPort->connectTo(sourcePort);
    portConnections_.push_back(new PortConnection(sourcePort, destPort));
    modified();
}    

void ProcessorNetwork::disconnectPorts(Port* sourcePort, Port* destPort) {
    //TODO: remove connection information from sourcePort and destPort
    for (size_t i=0; i<portConnections_.size(); i++) {            
        if ( (portConnections_[i]->getInport()==sourcePort && portConnections_[i]->getOutport()==destPort) ||
             (portConnections_[i]->getOutport()==sourcePort && portConnections_[i]->getInport()==destPort)
            ) {                
                 portConnections_.erase(portConnections_.begin()+i);
                 break;
            }
    }        
    modified();
}

ProcessorLink* ProcessorNetwork::getProcessorLink(Processor* sourceProcessor, Processor* destProcessor) {
    ProcessorLink* link=0;
    for (size_t i=0; i<processorLinks_.size(); i++) { 
        if ( (processorLinks_[i]->getInProcessor()==sourceProcessor && processorLinks_[i]->getOutProcessor()==destProcessor) ||
             (processorLinks_[i]->getOutProcessor()==sourceProcessor && processorLinks_[i]->getInProcessor()==destProcessor)
            ) {
                link = processorLinks_[i];
                break;
        }
    }
    return link;
}

void ProcessorNetwork::addLink(Processor* sourceProcessor, Processor* destProcessor) {        
    processorLinks_.push_back(new ProcessorLink(sourceProcessor, destProcessor));
    modified();
}

void ProcessorNetwork::removeLink(Processor* sourceProcessor, Processor* destProcessor) {
    for (size_t i=0; i<processorLinks_.size(); i++) {            
        if ((processorLinks_[i]->getInProcessor()==sourceProcessor && processorLinks_[i]->getOutProcessor()==destProcessor) ||
            (processorLinks_[i]->getOutProcessor()==sourceProcessor && processorLinks_[i]->getInProcessor()==destProcessor)
            ) {                
                processorLinks_.erase(processorLinks_.begin()+i);
                break;
        }
    }
    modified();
}

ProcessorNetwork::ProcessorNetwork() {
    isModified_ = true;
}

ProcessorNetwork::~ProcessorNetwork() {}

void ProcessorNetwork::addProcessor(Processor* processor) {
    if(std::find(processors_.begin(), processors_.end(), processor)== processors_.end())
        processors_.push_back(processor);
    modified();
}

void ProcessorNetwork::removeProcessor(Processor* processor) {
    // remove all connections to other processors
    std::vector<PortConnection*> eraseList;
    std::vector<Port*> outports = processor->getOutports();
    for (size_t i=0; i<portConnections_.size(); i++) {
        for (size_t j=0; j<outports.size(); j++) {
            if (portConnections_[i]->getOutport() == outports[j]) {
                outports[j]->disconnectFrom(portConnections_[i]->getInport());
                eraseList.push_back(portConnections_[i]);
            }
        }
    }
    for (size_t i=0; i<eraseList.size(); i++) {
        for (size_t j=0; j<portConnections_.size(); j++) {
            if (portConnections_[j] == eraseList[i])
                portConnections_.erase(portConnections_.begin()+j);
        }
    }

    eraseList.clear();
    std::vector<Port*> inports = processor->getInports();
    for (size_t i=0; i<portConnections_.size(); i++) {
        for (size_t j=0; j<inports.size(); j++) {
            if (portConnections_[i]->getInport() == inports[j]) {
                inports[j]->disconnectFrom(portConnections_[i]->getOutport());
                eraseList.push_back(portConnections_[i]);
            }
        }
    }
    for (size_t i=0; i<eraseList.size(); i++) {
        for (size_t j=0; j<portConnections_.size(); j++) {
            if (portConnections_[j] == eraseList[i])
                portConnections_.erase(portConnections_.begin()+j);
        }
    }

    // remove processors itself
    processors_.erase(std::remove(processors_.begin(), processors_.end(),
                      processor), processors_.end());

    modified();
}

Processor* ProcessorNetwork::getProcessorByName(std::string name) const {
    for (size_t i=0; i<processors_.size(); i++)
        if ((processors_[i]->getIdentifier()).compare(name) == 0)
            return processors_[i];
    return 0;
    // TODO: throw not found exception
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

    for (size_t i=0; i<portConnections.size(); i++) {
        connectPorts(portConnections[i]->getOutport(), portConnections[i]->getInport());
    }

    for (size_t i=0; i<processorLinks.size(); i++) {
        addLink(processorLinks[i]->getOutProcessor(), processorLinks[i]->getInProcessor());
    }
}

} // namespace
