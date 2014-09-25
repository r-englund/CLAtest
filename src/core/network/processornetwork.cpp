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
    KeepTrueWhileInScope(bool* b) : variable_(b) { (*variable_) = true; }
    ~KeepTrueWhileInScope() { (*variable_) = false; }
private:
    bool* variable_;
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
    , linking_(false) {
    linkEvaluator_ = new LinkEvaluator();
}

ProcessorNetwork::~ProcessorNetwork() {
    for (size_t i=0; i<processors_.size(); i++)
        delete processors_[i];
    delete linkEvaluator_;
}

void ProcessorNetwork::addProcessor(Processor* processor) {
    notifyObserversProcessorNetworkWillAddProcessor(processor);
    processors_.push_back(processor);
    processor->ProcessorObservable::addObserver(this);
    modified();
    notifyObserversProcessorNetworkDidAddProcessor(processor);
}

void ProcessorNetwork::removeProcessor(Processor* processor) {
    notifyObserversProcessorNetworkWillRemoveProcessor(processor);

    // Remove all connections for this processor
    std::vector<Outport*> outports;
    std::vector<Inport*> inports;
    outports = processor->getOutports();
    for (int i = 0; i < outports.size(); ++i) {
        inports = outports[i]->getConnectedInports();
        for (int j = 0; j < inports.size(); ++j) {
            removeConnection(outports[i], inports[j]);
        }
    }
    inports = processor->getInports();
    for (int i = 0; i < outports.size(); ++i) {
        outports = inports[i]->getConnectedOutports();
        for (int j = 0; j < inports.size(); ++j) {
            removeConnection(outports[i], inports[j]);
        }
    }

    // Remove all links for this processor
    std::vector<PropertyLink*> propertyLinks = propertyLinks_;
    for (size_t i = 0; i < propertyLinks.size(); i++) {
        if (propertyLinks[i]->getSourceProperty()->getOwner()->getProcessor() == processor ||
            propertyLinks[i]->getDestinationProperty()->getOwner()->getProcessor() == processor) {
            removeLink(propertyLinks[i]->getSourceProperty(),
                       propertyLinks[i]->getDestinationProperty());
        }
    }

    // remove processor itself
    processors_.erase(std::remove(processors_.begin(), processors_.end(), processor),
                      processors_.end());
    processor->ProcessorObservable::removeObserver(this);
    modified();
    notifyObserversProcessorNetworkDidRemoveProcessor(processor);
}

void ProcessorNetwork::removeAndDeleteProcessor(Processor* processor) {
    removeProcessor(processor);
    if (processor->isInitialized()) {
        processor->deinitialize();
    }
    delete processor;
}

Processor* ProcessorNetwork::getProcessorByName(std::string identifier) const {
    for (size_t i = 0; i < processors_.size(); i++)
        if (processors_[i]->getIdentifier() == identifier)
            return processors_[i];

    return NULL;
}

std::vector<Processor*> ProcessorNetwork::getProcessors() const {
    return processors_;
}


PortConnection* ProcessorNetwork::addConnection(Outport* sourcePort, Inport* destPort) {
    PortConnection* connection = getConnection(sourcePort, destPort);

    if (!connection && sourcePort && destPort && destPort->canConnectTo(sourcePort)) {
        notifyObserversProcessorNetworkWillAddConnection(connection);

        connection = new PortConnection(sourcePort, destPort);
        portConnections_[std::make_pair(sourcePort,destPort)] = connection;
        modified();
        destPort->connectTo(sourcePort);
        
        notifyObserversProcessorNetworkDidAddConnection(connection);
    }

    return connection;
}

void ProcessorNetwork::removeConnection(Outport* sourcePort, Inport* destPort) {
    PortConnectionMap::iterator it = portConnections_.find(std::make_pair(sourcePort, destPort));
    if (it != portConnections_.end()) {
        PortConnection* connection = it->second;
        notifyObserversProcessorNetworkWillRemoveConnection(connection);

        modified();
        destPort->disconnectFrom(sourcePort);
        portConnections_.erase(it);

        notifyObserversProcessorNetworkDidRemoveConnection(connection);
        delete connection;
    }
}

bool ProcessorNetwork::isConnected(Outport* sourcePort, Inport* destPort) {
    if (getConnection(sourcePort, destPort))
        return true;

    return false;
}

PortConnection* ProcessorNetwork::getConnection(Outport* sourcePort, Inport* destPort) {
    PortConnectionMap::iterator it = portConnections_.find(std::make_pair(sourcePort, destPort));
    if (it != portConnections_.end()) {
        return it->second;
    }
    return NULL;
}

std::vector<PortConnection*> ProcessorNetwork::getConnections() const {
    PortConnectionVector connections_;
    for (PortConnectionMap::const_iterator it = portConnections_.begin(); it != portConnections_.end();
         ++it) {
        connections_.push_back(it->second);
    }
    return connections_;
}

///////////////////////////////////////////////////////////////////////
//TODO: ProcessorLinks are Deprecated. To be removed

ProcessorLink* ProcessorNetwork::addLink(PropertyOwner* sourceProcessor, PropertyOwner* destProcessor) {
    ProcessorLink* link = getProcessorLink(sourceProcessor, destProcessor);

    if (!link) {
        link = new ProcessorLink(sourceProcessor, destProcessor);
        processorLinks_.push_back(link);
        modified();
    }

    return link;
}

void ProcessorNetwork::removeLink(PropertyOwner* sourceProcessor, PropertyOwner* destProcessor) {
    for (size_t i = 0; i < processorLinks_.size(); i++) {
        if ((processorLinks_[i]->getSourceProcessor() == sourceProcessor &&
             processorLinks_[i]->getDestinationProcessor() == destProcessor) ||
            (processorLinks_[i]->getDestinationProcessor() == sourceProcessor &&
             processorLinks_[i]->getSourceProcessor() == destProcessor)) {
            std::vector<PropertyLink*> plinks = processorLinks_[i]->getPropertyLinks();
            for (size_t j=0; j<plinks.size(); j++) {
                removeLink(plinks[j]->getSourceProperty(), plinks[j]->getDestinationProperty());
            }
            delete processorLinks_[i];
            processorLinks_.erase(processorLinks_.begin() + i);
            modified();
            break;
        }
    }
}

bool ProcessorNetwork::isLinked(PropertyOwner* src, PropertyOwner* dst) {
    if (getProcessorLink(src, dst))
        return true;

    return false;
}

ProcessorLink* ProcessorNetwork::getProcessorLink(PropertyOwner* processor1, PropertyOwner* processor2) const {
    for (size_t i = 0; i < processorLinks_.size(); i++) {
        if ((processorLinks_[i]->getSourceProcessor() == processor1 &&
             processorLinks_[i]->getDestinationProcessor() == processor2) ||
            (processorLinks_[i]->getDestinationProcessor() == processor1 &&
             processorLinks_[i]->getSourceProcessor() == processor2))
            return processorLinks_[i];
    }

    return NULL;
}

std::vector<ProcessorLink*> ProcessorNetwork::getProcessorLinks() const {
    return processorLinks_;
}

///////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
//Property Links

PropertyLink* ProcessorNetwork::addLink(Property* sourceProperty, Property* destinationProperty) {
    PropertyLink* link = getLink(sourceProperty, destinationProperty);
    if (!link) {
        link = new PropertyLink(sourceProperty, destinationProperty);
        propertyLinks_.push_back(link);
        addToPrimaryCache(link); //add to cache
        modified();
    }
    return link;
}

void ProcessorNetwork::removeLink(Property* sourceProperty, Property* destinationProperty) {
    for (size_t i = 0; i < propertyLinks_.size(); i++) {
        if ((propertyLinks_[i]->getSourceProperty() == sourceProperty &&
            propertyLinks_[i]->getDestinationProperty() == destinationProperty)) {
                removeFromPrimaryCache(propertyLinks_[i]); //remove from cache
                delete propertyLinks_[i];
                propertyLinks_.erase(propertyLinks_.begin() + i);
                modified();
                break;
        }
    }
}

bool ProcessorNetwork::isLinked(Property* sourceProperty, Property* destinationProperty) {
    if (getLink(sourceProperty, sourceProperty))
        return true;
    return false;
}

PropertyLink* ProcessorNetwork::getLink(Property* sourceProperty, Property* destinationProperty) const {
    for (size_t i = 0; i < propertyLinks_.size(); i++) {
        if ((propertyLinks_[i]->getSourceProperty() == sourceProperty &&
            propertyLinks_[i]->getDestinationProperty() == destinationProperty))
            return propertyLinks_[i];
    }
    return NULL;
}

std::vector<PropertyLink*> ProcessorNetwork::getLinks() const {
    return propertyLinks_;
}

void ProcessorNetwork::removeBidirectionalPair(Property* startProperty, Property* endProperty) {
    PropertyLink* pair = getBidirectionalPair(startProperty, endProperty);
    if (pair) removeLink(pair->getSourceProperty(), pair->getDestinationProperty());
}

PropertyLink* ProcessorNetwork::getBidirectionalPair(Property* startProperty, Property* endProperty) {
    PropertyLink* link = getLink(startProperty, endProperty);
    if (link)  return getLink(endProperty, startProperty);
    return 0;
}

void ProcessorNetwork::setLinkModifiedByOwner(PropertyOwner *processor) {
    //TODO: searching through vector is expensive, hence this function should not be called frequently
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor() == processor)
            propertyLinks_[i]->getSourceProperty()->propertyModified();
        if (propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor() == processor)
            propertyLinks_[i]->getDestinationProperty()->propertyModified();
    }
}

std::vector<PropertyLink*> ProcessorNetwork::getLinksBetweenProcessors(PropertyOwner* sourceProcessor, PropertyOwner* destinationProcessor) {
    //TODO: searching through vector is expensive, hence this function should not be called frequently
    std::vector<PropertyLink*> links;
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if ( (propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor() == sourceProcessor && 
             propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor() == destinationProcessor) ||
             (propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor() == sourceProcessor && 
             propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor() == destinationProcessor)
           )
            links.push_back(propertyLinks_[i]);
    }
    return links;
}

void ProcessorNetwork::addToPrimaryCache(PropertyLink* propertyLink) {
    Property* srcProperty = propertyLink->getSourceProperty();
    Property* dstProperty = propertyLink->getDestinationProperty();

    //Update primary cache
    std::vector<Property*> propertiesLinkedToSource = propertyLinkPrimaryCache_[srcProperty];
    if ( std::find(propertiesLinkedToSource.begin(), propertiesLinkedToSource.end(),
                   dstProperty)==propertiesLinkedToSource.end()) {
       propertiesLinkedToSource.push_back(dstProperty);
    }

    propertyLinkPrimaryCache_[srcProperty] = propertiesLinkedToSource;

    if (propertiesLinkedToSource.size()==0) {
        propertyLinkPrimaryCache_.erase(srcProperty);
    }

    clearSecondaryCache();
}

void ProcessorNetwork::removeFromPrimaryCache(PropertyLink* propertyLink) {
    Property* srcProperty = propertyLink->getSourceProperty();
    Property* dstProperty = propertyLink->getDestinationProperty();

    //Update primary cache
    std::vector<Property*> propertiesLinkedToSource = propertyLinkPrimaryCache_[srcProperty];
    std::vector<Property*>::iterator sIt = std::find(propertiesLinkedToSource.begin(), 
                                                     propertiesLinkedToSource.end(),
                                                     dstProperty);
    if ( sIt!= propertiesLinkedToSource.end()) {
        propertiesLinkedToSource.erase(sIt);
    }
    propertyLinkPrimaryCache_[srcProperty] = propertiesLinkedToSource;

    if (propertiesLinkedToSource.size()==0) {
        propertyLinkPrimaryCache_.erase(srcProperty);
    }

    clearSecondaryCache();
}

void ProcessorNetwork::updatePropertyLinkCaches() {
    clearSecondaryCache();
    propertyLinkPrimaryCache_.clear();
    for (size_t i=0; i<propertyLinks_.size(); i++)
        addToPrimaryCache(propertyLinks_[i]);

    /*
    //Debug info
    std::string info("Property Link Cache Info: \n");
    for (std::map<Property*, std::vector<Property*> >::iterator it=propertyLinkPrimaryCache_.begin();
        it!=propertyLinkPrimaryCache_.end(); ++it) {
            info += it->first->getIdentifier() + " : ";
            std::vector<Property*> linkedProperties = getLinkedProperties(it->first) ;
            for (size_t i=0; i<linkedProperties.size(); i++) {
                info += linkedProperties[i]->getIdentifier();
                info += " ";
            }
            info += "\n";
    }
    LogWarn(info);
    */
}

void ProcessorNetwork::clearSecondaryCache() {
    propertyLinkSecondaryCache_.clear();
}

std::vector<Property*> ProcessorNetwork::getLinkedProperties(Property* property) {

    //check if link connectivity has been computed and cached already
    if (propertyLinkSecondaryCache_.find(property)!=propertyLinkSecondaryCache_.end()) {
        return propertyLinkSecondaryCache_[property];
    }

    //compute link connectivity using primary cache
    std::vector<Property*> properties = propertyLinkPrimaryCache_[property];
    std::vector<Property*> linkedProperties;
    while (properties.size()) {
        std::vector<Property*> tempProperties;
        for (size_t i=0; i<properties.size(); i++) {
            if ( std::find(linkedProperties.begin(), linkedProperties.end(),
                properties[i])==linkedProperties.end() && properties[i]!=property) {
                    linkedProperties.push_back(properties[i]);

                    std::vector<Property*> p = propertyLinkPrimaryCache_[properties[i]];
                    for (size_t j=0; j<p.size(); j++) {
                        if ( std::find(tempProperties.begin(), tempProperties.end(),
                            p[j])==tempProperties.end() && p[j]!=property) {
                                tempProperties.push_back(p[j]);
                        }
                    }
            }
        }
        properties = tempProperties;
    };

    //store connectivity in secondary cache so that subsequent call does not recompute connectivity
    propertyLinkSecondaryCache_[property] = linkedProperties;
    return linkedProperties;
}

/////////////////////////////////////////////////////////////////////////////////////////

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

bool ProcessorNetwork::isLinking() const{
    return linking_;
}

bool ProcessorNetwork::isInvalidating() const { 
    return invalidating_; 
}

void ProcessorNetwork::onProcessorInvalidationBegin(Processor* p) {
    std::vector<Processor*>::iterator it =
        std::find_if(processorsInvalidating_.begin(), processorsInvalidating_.end(),
                     ComparePointers<Processor>(p));

    if (it != processorsInvalidating_.end()) return;

    processorsInvalidating_.push_back(p);

    if (!isInvalidating()) {
        invalidating_ = true;
    }
}

void ProcessorNetwork::onProcessorInvalidationEnd(Processor* p) {
    std::vector<Processor*>::iterator it =
        std::find_if(processorsInvalidating_.begin(), processorsInvalidating_.end(),
        ComparePointers<Processor>(p));

    if (it != processorsInvalidating_.end()) processorsInvalidating_.erase(it);

    if (processorsInvalidating_.empty()) {
        invalidating_ = false;

        if (evaluationQueued_ && !isLinking()) {
            notifyObserversProcessorNetworkEvaluateRequest();
            evaluationQueued_ = false;
        }
    }
}

void ProcessorNetwork::onProcessorRequestEvaluate(Processor*) {
    if (isLinking() || isInvalidating())
        evaluationQueued_ = true;
    else {
        notifyObserversProcessorNetworkEvaluateRequest();
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
    notifyObserversProcessorNetworkChanged();
}

void ProcessorNetwork::performLinkingOnPropertyChange(Property* modifiedProperty) {
    if (modifiedProperty)
        evaluatePropertyLinks(modifiedProperty);
}


void ProcessorNetwork::evaluatePropertyLinks(Property* modifiedProperty) {
    if (isLinking()) return;

    lock();
    linking_ = true;
    //perform linking
    //TODO: Yet to add another level of caching for optimization.
    std::vector<Property*> destinationProperties = getLinkedProperties(modifiedProperty);

    for (size_t i=0; i<destinationProperties.size(); i++) {
        if (modifiedProperty!=destinationProperties[i])
            linkEvaluator_->evaluate(modifiedProperty, destinationProperties[i]);
    }

    unlock();

    if (isLinking()) linking_ = false;
}

void ProcessorNetwork::evaluatePropertyLinks1(Property* modifiedProperty) {
    if (isLinking())
        return;

    lock();
    linking_ = true;
    //perform linking
    std::vector<ProcessorLink*> sortedModifiableLinks = getSortedProcessorLinksFromProperty(modifiedProperty);
    //This saves expensive branched search. But can be still optimized by caching.
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

    if (isLinking()) {
        linking_ = false;

//         if (evaluationQueued_ && linkInvalidationInitiator_!=getInvalidationInitiator())
//             onProcessorRequestEvaluate(linkInvalidationInitiator_);
// 
//         linkInvalidationInitiator_ = NULL;
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
            PropertyOwner* srcProc = sortedProcessorLinks[k]->getSourceProcessor();
            PropertyOwner* dstProc = sortedProcessorLinks[k]->getDestinationProcessor();

            //Find link connected to the seeds
            for (size_t i=0; i<unsortedProcessorLinks.size(); i++) {
                nextInvalidLink = 0;

                if (std::find(sortedProcessorLinks.begin(), sortedProcessorLinks.end(),
                              unsortedProcessorLinks[i])==sortedProcessorLinks.end()) {

                    std::vector<PropertyLink*> propertyLinks = unsortedProcessorLinks[i]->getPropertyLinks();

                    //Optimized
                    for (size_t j=0; j<propertyLinks.size(); j++) {
                        if (propertyLinks[j]->getSourceProperty()->getOwner()->getProcessor() == srcProc || 
                            propertyLinks[j]->getDestinationProperty()->getOwner()->getProcessor() == dstProc ||
                            propertyLinks[j]->getSourceProperty()->getOwner()->getProcessor() == dstProc ||
                            propertyLinks[j]->getSourceProperty()->getOwner()->getProcessor() == srcProc) 
                        {
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
    s.serialize("ProcessorNetworkVersion", processorNetworkVersion_);
    s.serialize("Processors", processors_, "Processor");
    s.serialize("Connections", getConnections(), "Connection");
    s.serialize("PropertyLinks", propertyLinks_, "PropertyLink");
}

void ProcessorNetwork::deserialize(IvwDeserializer& d) throw (Exception) {
    // This will set deserializing_ to true while keepTrueWillAlive is in scope
    // and set it to false no matter how we leave the scope
    KeepTrueWhileInScope keepTrueWillAlive(&deserializing_);
    std::vector<PortConnection*> portConnections;
    std::vector<ProcessorLink*> processorLinks;

    int version = 0;
    d.deserialize("ProcessorNetworkVersion", version);

    if(version != processorNetworkVersion_) {
        LogWarn("Old network version, performing updates (" << version << " to "
                << processorNetworkVersion_ << ") file: " << d.getFileName());
        NetworkConverter nv(version);
        d.convertVersion(&nv); 
    }

    //Processors
    try {
        d.deserialize("Processors", processors_, "Processor");
        int i = 1;
        std::vector<Processor*>::iterator it = processors_.begin();

        while (it != processors_.end()) {
            if (*it) {
                (*it)->ProcessorObservable::addObserver(this);
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

        ///////////////////////////////////////////////////////////////////////
        //TODO: ProcessorLinks are Deprecated. To be removed
        d.deserialize("ProcessorLinks", processorLinks, "ProcessorLink");

        if (processorLinks.size())
            LogWarn("ProcessorLinks are deprecated. Recommended to resave workspace.");

        for (size_t i=0; i<processorLinks.size(); i++) {
            if (processorLinks[i]) {
                PropertyOwner* inProcessor = processorLinks[i]->getSourceProcessor();
                PropertyOwner* outProcessor = processorLinks[i]->getDestinationProcessor();

                if (inProcessor && outProcessor) {
                    std::vector<PropertyLink*> propertyLinks = processorLinks[i]->getPropertyLinks();

                    for (size_t j=0; j<propertyLinks.size(); j++) {
                        Property* srcProperty = propertyLinks[j]->getSourceProperty();
                        Property* dstProperty = propertyLinks[j]->getDestinationProperty();

                        if (!srcProperty || !dstProperty) {
                            processorLinks[i]->removePropertyLinks(srcProperty, dstProperty);
                            LogWarn("Unable to establish property link.");
                        }
                    }

                    propertyLinks = processorLinks[i]->getPropertyLinks();
                    for (size_t j=0; j<propertyLinks.size(); j++)
                        propertyLinks_.push_back(propertyLinks[j]);

                    if (processorLinks[i]->getPropertyLinks().size())
                        processorLinks_.push_back(processorLinks[i]);
                } else
                    LogWarn("Unable to establish processor link.");
            } else
                LogWarn("Failed deserialization: Processor Links Nr." << i);
        }

        ///////////////////////////////////////////////////////////////////////

        std::vector<PropertyLink*> propertyLinks;
        d.deserialize("PropertyLinks", propertyLinks, "PropertyLink");
        for (size_t j=0; j<propertyLinks.size(); j++) {
            if (propertyLinks[j]->getSourceProperty() && propertyLinks[j]->getDestinationProperty()) {
                propertyLinks_.push_back(propertyLinks[j]);
            } else {
                LogWarn("Unable to establish property link.");
            }
        }

        updatePropertyLinkCaches();

        ///////////////////////////////////////////////////////////////////////
        //TODO: ProcessorLinks are Deprecated. To be removed
        for (size_t i=0; i<propertyLinks_.size(); i++) {
            
            PropertyOwner* srcProcessor = propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor();
            PropertyOwner* dstProcessor = propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor();

            ProcessorLink* processorLink = getProcessorLink(srcProcessor, dstProcessor);
            std::vector<PropertyLink*> plinks = getLinksBetweenProcessors(srcProcessor, dstProcessor);

            if (plinks.size()) {
                if (!processorLink) processorLink = addLink(srcProcessor, dstProcessor);
                for (size_t j=0; j<plinks.size(); j++) {
                    processorLink->addPropertyLinks(plinks[j]);
                }
            }
        }
        ///////////////////////////////////////////////////////////////////////

    } catch (const SerializationException& exception) {
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    } catch (...) {
        //Abort and clear network in case of unknown exception
        clear();
        throw AbortException("Unknown Exception.");
    }

    notifyObserversProcessorNetworkChanged();
}

bool ProcessorNetwork::isDeserializing()const {
    return deserializing_;
}

const int ProcessorNetwork::processorNetworkVersion_ = 1;


ProcessorNetwork::NetworkConverter::NetworkConverter(int from)
    : VersionConverter(), from_(from) {}

bool ProcessorNetwork::NetworkConverter::convert(TxElement* root) {
    switch (from_) {
        case 0:
            traverseNodes(root, &ProcessorNetwork::NetworkConverter::updateProcessorType);
        default:
            break;
    }
    
    return true;
}

void ProcessorNetwork::NetworkConverter::traverseNodes(TxElement* node, updateType update) {
    (this->*update)(node);
    ticpp::Iterator<ticpp::Element> child;
    for (child = child.begin(node); child != child.end(); child++) {
        traverseNodes(child.Get(), update);
    }
}

void ProcessorNetwork::NetworkConverter::updateProcessorType(TxElement* node) {
    std::string key;
    node->GetValue(&key);

    if (key == "Processor") {
        std::string type = node->GetAttributeOrDefault("type", "");
        if (splitString(type, '.').size() < 3){
            node->SetAttribute("type", "org.inviwo."+ type);   
        }
    }
}





} // namespace
