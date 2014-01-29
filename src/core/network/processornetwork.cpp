/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

ProcessorNetwork::ProcessorNetwork() : VoidObservable(), ProcessorObserver(),
    modified_(true),
	broadcastModification_(true),
    locked_(0),
    invalidating_(false),
    invalidationInitiator_(NULL) {}

ProcessorNetwork::~ProcessorNetwork() {
    clear();
}

void ProcessorNetwork::addProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)==processors_.end(),
              "Processor instance already contained in processor network.");

    processors_.push_back(processor);
    processor->addObserver(this);
    modified();
}

void ProcessorNetwork::removeAndDeleteProcessor(Processor* processor) {
    removeProcessor(processor);
    processor->deinitialize();
    delete processor;
}


void ProcessorNetwork::removeProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)!=processors_.end(),
              "Processor instance not contained in processor network.");

    // remove all connections for this processor
    std::vector<PortConnection*> portConnections = portConnections_;
    for (size_t i=0; i<portConnections.size(); i++)
        if (portConnections[i]->involvesProcessor(processor))
            removeConnection(portConnections[i]->getOutport(),
                             portConnections[i]->getInport());

    // remove all links for this processor
    std::vector<ProcessorLink*> processorLinks = processorLinks_;
    for (size_t i=0; i<processorLinks.size(); i++)
        if (processorLinks[i]->involvesProcessor(processor))
            removeLink(processorLinks[i]->getDestinationProcessor(),
                       processorLinks[i]->getSourceProcessor());

    // remove processor itself
    processors_.erase(std::remove(processors_.begin(), processors_.end(), processor), processors_.end());
    processor->removeObserver(this);

    modified();
}


PortConnection* ProcessorNetwork::addConnection(Outport* sourcePort, Inport* destPort) {
    PortConnection* connection = getConnection(sourcePort, destPort);
    if(!connection) {
        lock();
        destPort->connectTo(sourcePort);
        connection = new PortConnection(sourcePort, destPort);
        portConnections_.push_back(connection);
        unlock();
        modified();
    }
    return connection;
}

void ProcessorNetwork::removeConnection(Outport* sourcePort, Inport* destPort) {
    lock();
    for (size_t i=0; i<portConnections_.size(); i++) {
        if (portConnections_[i]->getOutport()==sourcePort &&
            portConnections_[i]->getInport()==destPort) {

            destPort->disconnectFrom(sourcePort);
            delete portConnections_[i];
            portConnections_.erase(portConnections_.begin()+i);
            break;
        }
    }
    unlock();
    modified();
}

bool ProcessorNetwork::isLinked(Processor* src, Processor* dst) {
    if (getLink(src, dst))
        return true;
    return false;
}

ProcessorLink* ProcessorNetwork::addLink(Processor* sourceProcessor, Processor* destProcessor) {
    ProcessorLink* link = getLink(sourceProcessor, destProcessor);
    if(!link) {
        link = new ProcessorLink(sourceProcessor, destProcessor);
        processorLinks_.push_back(link);
        modified();
    }
    return link;
}

void ProcessorNetwork::removeLink(Processor* sourceProcessor, Processor* destProcessor) {
    for (size_t i=0; i<processorLinks_.size(); i++) {            
        if ((processorLinks_[i]->getSourceProcessor()==sourceProcessor &&
             processorLinks_[i]->getDestinationProcessor()==destProcessor) ||
            (processorLinks_[i]->getDestinationProcessor()==sourceProcessor &&
             processorLinks_[i]->getSourceProcessor()==destProcessor)) {

            delete processorLinks_[i];
            processorLinks_.erase(processorLinks_.begin()+i);
            break;
        }
    }
    modified();
}

void ProcessorNetwork::clear() {
    std::vector<Processor*> processors = processors_;   
    for (size_t i=0; i<processors.size(); i++)
        removeAndDeleteProcessor(processors[i]);
}

Processor* ProcessorNetwork::getProcessorByName(std::string identifier) const {
    for (size_t i=0; i<processors_.size(); i++)
        if (processors_[i]->getIdentifier()==identifier)
            return processors_[i];
    return 0;
}

ProcessorLink* ProcessorNetwork::getLink(Processor* processor1, Processor* processor2) const {
    for (size_t i=0; i<processorLinks_.size(); i++) { 
        if ((processorLinks_[i]->getSourceProcessor()==processor1 &&
             processorLinks_[i]->getDestinationProcessor()==processor2) ||
            (processorLinks_[i]->getDestinationProcessor()==processor1 &&
             processorLinks_[i]->getSourceProcessor()==processor2)) {
            return processorLinks_[i];
        }
    }    
    return NULL;
}

void ProcessorNetwork::modified() {
	modified_ = true; 
	if (broadcastModification_) {
		notifyObservers();
	}	
}

void ProcessorNetwork::notifyInvalidationBegin(Processor* p){
    if(!isInvalidating()){       
        invalidationInitiator_ = p;
        invalidating_ = true;
        //TODO: Optimize notification only to network evaluator. 
        //(But for now notify for all observers. Currently there are only two observers observing network)
        notifyObservers();
    }
}

void ProcessorNetwork::notifyInvalidationEnd(Processor* p){
    if(invalidationInitiator_ == p){
        invalidating_ = false;
        invalidationInitiator_ = NULL;
    }
}

void ProcessorNetwork::notifyRequestEvaluate(Processor*){
    notifyObservers();
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
        int i = 1;
        std::vector<Processor*>::iterator it = processors_.begin();
        while (it != processors_.end()) {
            if(*it){
                (*it)->addObserver(this);
                ++it;
            } else {
                it = processors_.erase(it);
                LogWarn("Failed deserialization: Processor Nr." << i);
            }
            ++i;
        }
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
            if(portConnections[i]){
                Outport* outPort = portConnections[i]->getOutport();
                Inport* inPort = portConnections[i]->getInport();
                if (outPort && inPort) {
                    addConnection(outPort, inPort);
                } else {
                    LogWarn("Unable to establish port connection.");
                }
                delete portConnections[i];
            } else {
                LogWarn("Failed deserialization: Port Connection Nr." << i);
            }
        }
    } catch (const SerializationException& exception) {             
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    } catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception."); 
    }

    //Links
    try {        
        d.deserialize("ProcessorLinks", processorLinks, "ProcessorLink");        
        for (size_t i=0; i<processorLinks.size(); i++) {
            if(processorLinks[i]){
                Processor* inProcessor = processorLinks[i]->getSourceProcessor();
                Processor* outProcessor = processorLinks[i]->getDestinationProcessor();
                if (inProcessor && outProcessor) {
                    std::vector<PropertyLink*> propertyLinks = processorLinks[i]->getPropertyLinks();
                    for (size_t j=0; j<propertyLinks.size(); j++) {
                        Property* srcProperty = propertyLinks[j]->getSourceProperty();
                        Property* dstProperty = propertyLinks[j]->getDestinationProperty();
                        if (!srcProperty || !dstProperty) {
                            processorLinks[i]->removePropertyLink(propertyLinks[j]);
                            LogWarn("Unable to establish property link.");
                        }
                    }
                    if(processorLinks[i]->getPropertyLinks().size()) {
                        processorLinks_.push_back(processorLinks[i]);
                    }
                } else {
                    LogWarn("Unable to establish processor link.");
                }
            } else {
                LogWarn("Failed deserialization: Processor Links Nr." << i);
            }
        }        
    } catch (const SerializationException& exception) {       
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    } catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception.");        
    }
	notifyObservers();

}

std::vector<PortConnection*> ProcessorNetwork::getConnections() const {
    return portConnections_;
}

std::vector<Processor*> ProcessorNetwork::getProcessors() const {
    return processors_;
}

std::vector<ProcessorLink*> ProcessorNetwork::getLinks() const {
    return processorLinks_;
}

void ProcessorNetwork::setBroadcastModification(bool broadcastModification) {
    broadcastModification_ = broadcastModification;
}

PortConnection* ProcessorNetwork::getConnection(Outport* sourcePort, Inport* destPort) {
    for(size_t i = 0; i < portConnections_.size(); i++) {
        if(portConnections_[i]->getOutport() == sourcePort &&
           portConnections_[i]->getInport() == destPort) {
            return portConnections_[i];
        }
    }
    return NULL;
}

bool ProcessorNetwork::isConnected(Outport* sourcePort, Inport* destPort) {
    if(getConnection(sourcePort, destPort))
        return true;
    return false;
}

} // namespace
