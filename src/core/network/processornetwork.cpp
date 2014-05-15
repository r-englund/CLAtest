/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/util/vectoroperations.h>
#include <algorithm>

namespace inviwo {

namespace {
class KeepTrueWhileInScope {
public:
    KeepTrueWhileInScope(bool* b) :variable_(b) {
        (*variable_) = true;
    }

    ~KeepTrueWhileInScope() {
        (*variable_) = false;
    }

private:
    bool* variable_;;
};
}

ProcessorNetwork::ProcessorNetwork()
    : ProcessorNetworkObservable()
    , ProcessorObserver()
    , modified_(true)
    , locked_(0)
    , deserializing_(false)
    , invalidating_(false)
    , linkEvaluator_(NULL)
    , evaluationQueued_(false)
    , linking_(false)
    , linkInvalidationInitiator_(NULL) {
    linkEvaluator_ = new LinkEvaluator();
}

ProcessorNetwork::~ProcessorNetwork() {
    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    delete linkEvaluator_;
}

void ProcessorNetwork::addProcessor(Processor* processor) {
    ivwAssert(std::find(processors_.begin(), processors_.end(), processor)==processors_.end(),
              "Processor instance already contained in processor network.");
    processors_.push_back(processor);
    processor->addObserver(this);
    modified();
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

void ProcessorNetwork::removeAndDeleteProcessor(Processor* processor) {
    removeProcessor(processor);
    processor->deinitialize();
    delete processor;
}

Processor* ProcessorNetwork::getProcessorByName(std::string identifier) const {
    for (size_t i = 0; i < processors_.size(); i++)
        if (processors_[i]->getIdentifier() == identifier)
            return processors_[i];

    return 0;
}

std::vector<Processor*> ProcessorNetwork::getProcessors() const {
    return processors_;
}


PortConnection* ProcessorNetwork::addConnection(Outport* sourcePort, Inport* destPort) {
    PortConnection* connection = getConnection(sourcePort, destPort);

    if (!connection && sourcePort && destPort && destPort->canConnectTo(sourcePort)) {
        connection = new PortConnection(sourcePort, destPort);
        portConnections_.push_back(connection);
        modified();
        destPort->connectTo(sourcePort);
        
    }

    return connection;
}

void ProcessorNetwork::removeConnection(Outport* sourcePort, Inport* destPort) {
    for (size_t i=0; i<portConnections_.size(); i++) {
        if (portConnections_[i]->getOutport()==sourcePort &&
            portConnections_[i]->getInport()==destPort) {
            modified();
            destPort->disconnectFrom(sourcePort);
            delete portConnections_[i];
            portConnections_.erase(portConnections_.begin()+i);
            break;
        }
    }
}

bool ProcessorNetwork::isConnected(Outport* sourcePort, Inport* destPort) {
    if (getConnection(sourcePort, destPort))
        return true;

    return false;
}

PortConnection* ProcessorNetwork::getConnection(Outport* sourcePort, Inport* destPort) {
    for (size_t i = 0; i < portConnections_.size(); i++) {
        if (portConnections_[i]->getOutport() == sourcePort &&
            portConnections_[i]->getInport() == destPort)
            return portConnections_[i];
    }

    return NULL;
}

std::vector<PortConnection*> ProcessorNetwork::getConnections() const {
    return portConnections_;
}


ProcessorLink* ProcessorNetwork::addLink(Processor* sourceProcessor, Processor* destProcessor) {
    ProcessorLink* link = getLink(sourceProcessor, destProcessor);

    if (!link) {
        link = new ProcessorLink(sourceProcessor, destProcessor);
        processorLinks_.push_back(link);
        modified();
    }

    return link;
}

void ProcessorNetwork::removeLink(Processor* sourceProcessor, Processor* destProcessor) {
    for (size_t i = 0; i < processorLinks_.size(); i++) {
        if ((processorLinks_[i]->getSourceProcessor() == sourceProcessor &&
             processorLinks_[i]->getDestinationProcessor() == destProcessor) ||
            (processorLinks_[i]->getDestinationProcessor() == sourceProcessor &&
             processorLinks_[i]->getSourceProcessor() == destProcessor)) {
            delete processorLinks_[i];
            processorLinks_.erase(processorLinks_.begin() + i);
            modified();
            break;
        }
    }
}

bool ProcessorNetwork::isLinked(Processor* src, Processor* dst) {
    if (getLink(src, dst))
        return true;

    return false;
}

ProcessorLink* ProcessorNetwork::getLink(Processor* processor1, Processor* processor2) const {
    for (size_t i = 0; i < processorLinks_.size(); i++) {
        if ((processorLinks_[i]->getSourceProcessor() == processor1 &&
             processorLinks_[i]->getDestinationProcessor() == processor2) ||
            (processorLinks_[i]->getDestinationProcessor() == processor1 &&
             processorLinks_[i]->getSourceProcessor() == processor2))
            return processorLinks_[i];
    }

    return NULL;
}

std::vector<ProcessorLink*> ProcessorNetwork::getLinks() const {
    return processorLinks_;
}

void ProcessorNetwork::clear() {
    std::vector<Processor*> processors = processors_;

    lock();

    //Invalidate inports to alert processors that they should stop their calculations.
    for (size_t p=0; p<processors.size(); p++){
        std::vector<Inport*> inports = processors[p]->getInports();
        for (size_t i=0; i<inports.size(); i++)
            inports[i]->invalidate(PropertyOwner::INVALID_OUTPUT);
    }

    for (size_t i=0; i<processors.size(); i++)
        removeAndDeleteProcessor(processors[i]);

    unlock();
}

void ProcessorNetwork::modified() {
    modified_ = true;
}


void ProcessorNetwork::setModified(bool modified) { 
    modified_ = modified; 
}

bool ProcessorNetwork::isModified() const { 
    return modified_; 
}


bool ProcessorNetwork::isInvalidating() const { 
    return invalidating_; 
}

void ProcessorNetwork::onProcessorInvalidationBegin(Processor* p) {
    std::vector<Processor*>::iterator it = std::find_if(processorsInvalidating_.begin(), processorsInvalidating_.end(), ComparePointers<Processor>(p));

    if(it != processorsInvalidating_.end())
        return;

    processorsInvalidating_.push_back(p);

    if (!isInvalidating()) {
        invalidating_ = true;

        if (linking_ && !linkInvalidationInitiator_)
            linkInvalidationInitiator_ = p;
    }
}

void ProcessorNetwork::onProcessorInvalidationEnd(Processor* p) {
    std::vector<Processor*>::iterator it = std::find_if(processorsInvalidating_.begin(), processorsInvalidating_.end(), ComparePointers<Processor>(p));

    if(it != processorsInvalidating_.end())
        processorsInvalidating_.erase(it);

    if (processorsInvalidating_.empty()) {
        invalidating_ = false;

        if(evaluationQueued_){
            notifyProcessorNetworkEvaluateRequestObservers();
            evaluationQueued_ = false;
        }
    }
}

void ProcessorNetwork::onProcessorRequestEvaluate(Processor*) {
    if (linking_ || isInvalidating())
        evaluationQueued_ = true;
    else {
        notifyProcessorNetworkEvaluateRequestObservers();
        evaluationQueued_ = false;
    }
}


Processor* ProcessorNetwork::getInvalidationInitiator() {
    if(processorsInvalidating_.empty())
        return NULL;
    else
        return processorsInvalidating_[0]; 
}

//linking helpers

void ProcessorNetwork::onAboutPropertyChange(Property* modifiedProperty) {
    performLinkingOnPropertyChange(modifiedProperty);
}

void ProcessorNetwork::performLinkingOnPropertyChange(Property* modifiedProperty) {
    if (modifiedProperty)
        evaluatePropertyLinks(modifiedProperty);
}

void ProcessorNetwork::evaluatePropertyLinks(Property* modifiedProperty) {
    if (linking_)
        return;

    lock();
    linking_ = true;
    //perform linking
    std::vector<ProcessorLink*> sortedModifiableLinks = getSortedProcessorLinksFromProperty(modifiedProperty);
    //This saves expensive branched search. But can be still optimized.
    std::vector<Property*> destinationProperties;

    for (size_t i=0; i<sortedModifiableLinks.size(); i++) {
        std::vector<PropertyLink*> propertyLinks = sortedModifiableLinks[i]->getPropertyLinks();

        for (size_t j=0; j<propertyLinks.size(); j++) {
            Property* srcProp = propertyLinks[j]->getSourceProperty();
            Property* dstProp = propertyLinks[j]->getDestinationProperty();

            if (srcProp == modifiedProperty ||
                std::find(destinationProperties.begin(), destinationProperties.end(),
                          srcProp)!=destinationProperties.end()) {
                if (std::find(destinationProperties.begin(), destinationProperties.end(),
                              dstProp)==destinationProperties.end() && dstProp!=modifiedProperty)
                    destinationProperties.push_back(dstProp);
            }
        }
    }

    for (size_t i=0; i<destinationProperties.size(); i++)
        linkEvaluator_->evaluate(modifiedProperty, destinationProperties[i]);

    unlock();

    if (linking_) {
        linking_ = false;

        if (evaluationQueued_ && linkInvalidationInitiator_!=getInvalidationInitiator())
            onProcessorRequestEvaluate(linkInvalidationInitiator_);

        linkInvalidationInitiator_ = NULL;
    }
}

std::vector<ProcessorLink*> ProcessorNetwork::getSortedProcessorLinksFromProperty(Property* modifiedProperty) {
    std::vector<ProcessorLink*> unsortedProcessorLinks = processorLinks_;
    std::vector<ProcessorLink*> sortedProcessorLinks;
    ivwAssert(modifiedProperty!=0, "Expected a valid property");
    ProcessorLink* nextInvalidLink = 0;

    //Performing N-Ring neighbor search

    //Find initial invalid link (whose property is currently modified)
    //Immediate connected links (1st ring)
    for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
        if (unsortedProcessorLinks[i]->isPropertySource(modifiedProperty)) {
            if (std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(), unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                nextInvalidLink = unsortedProcessorLinks[i];
                sortedProcessorLinks.push_back(nextInvalidLink);
            }
        }
    }

    if (!nextInvalidLink)  return sortedProcessorLinks; //return empty list

    //Find link connected to the invalid link.
    while (unsortedProcessorLinks.size()) {
        size_t unsortedSize = unsortedProcessorLinks.size();

        //Seed from sorted links
        for (size_t k=0; k<sortedProcessorLinks.size(); k++) {
            Processor* srcProc = sortedProcessorLinks[k]->getSourceProcessor();
            Processor* dstProc = sortedProcessorLinks[k]->getDestinationProcessor();

            //Find link connected to the seeds
            for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
                nextInvalidLink = 0;

                if (std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(),
                              unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {
                    std::vector<PropertyLink*> propertyLinks = unsortedProcessorLinks[i]->getPropertyLinks();

                    for (size_t j=0; j<propertyLinks.size(); j++) {
                        if (propertyLinks[j]->getSourceProperty()->getOwner() == dstProc ||
                            propertyLinks[j]->getSourceProperty()->getOwner() == srcProc) {
                            nextInvalidLink = unsortedProcessorLinks[i];
                            unsortedProcessorLinks.erase(unsortedProcessorLinks.begin()+i);
                            break;
                        }
                    }
                }

                if (nextInvalidLink)
                    break;
            }

            if (nextInvalidLink) {
                sortedProcessorLinks.push_back(nextInvalidLink);
                break;
            }
        }

        //there is no more connected links
        if (unsortedSize == unsortedProcessorLinks.size())
            break;
    }

    return sortedProcessorLinks;
}

void ProcessorNetwork::serialize(IvwSerializer& s) const {
    s.serialize("Processors", processors_, "Processor");
    s.serialize("Connections", portConnections_, "Connection");
    s.serialize("ProcessorLinks", processorLinks_, "ProcessorLink");
}

void ProcessorNetwork::deserialize(IvwDeserializer& d) throw (Exception) {
    // This will set deserializing_ to true while keepTrueWillAlive is in scope
    // and set it to false no matter how we leave the scope
    KeepTrueWhileInScope keepTrueWillAlive(&deserializing_);
    std::vector<PortConnection*> portConnections;
    std::vector<ProcessorLink*> processorLinks;

    //Processors
    try {
        d.deserialize("Processors", processors_, "Processor");
        int i = 1;
        std::vector<Processor*>::iterator it = processors_.begin();

        while (it != processors_.end()) {
            if (*it) {
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
            if (portConnections[i]) {
                Outport* outPort = portConnections[i]->getOutport();
                Inport* inPort = portConnections[i]->getInport();

                if (!(outPort && inPort && addConnection(outPort, inPort)))
                    LogWarn("Unable to establish port connection.");

                delete portConnections[i];
            } else
                LogWarn("Failed deserialization: Port Connection Nr." << i);
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
            if (processorLinks[i]) {
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

                    if (processorLinks[i]->getPropertyLinks().size())
                        processorLinks_.push_back(processorLinks[i]);
                } else
                    LogWarn("Unable to establish processor link.");
            } else
                LogWarn("Failed deserialization: Processor Links Nr." << i);
        }
    } catch (const SerializationException& exception) {
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    } catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception.");
    }

    notifyProcessorNetworkObservers();
}

bool ProcessorNetwork::isDeserializing()const {
    return deserializing_;
}

} // namespace
