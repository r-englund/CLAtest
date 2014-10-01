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
#include <inviwo/core/util/settings/linksettings.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/links/linkconditions.h>
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

ProcessorPair::ProcessorPair(Processor* p1, Processor* p2) 
: processor1_(p1<p2?p1:p2), processor2_(p1<p2?p2:p1) {}

bool operator==(const ProcessorPair& p1, const ProcessorPair& p2) {
    return p1.processor1_ == p2.processor1_ && p1.processor2_ == p2.processor2_;
}

bool operator<(const ProcessorPair& p1, const ProcessorPair& p2) {
    if(p1.processor1_ != p2.processor1_) {
        return p1.processor1_ < p2.processor1_;
    } else {
        return p1.processor2_ < p2.processor2_;
    }
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
    for (ProcessorMap::iterator it = processors_.begin(); it != processors_.end(); ++it) {
        delete it->second;
    }
    processors_.clear();
    delete linkEvaluator_;
}

void ProcessorNetwork::addProcessor(Processor* processor) {
    notifyObserversProcessorNetworkWillAddProcessor(processor);
    processors_[processor->getIdentifier()] = processor;
    processor->ProcessorObservable::addObserver(this);
    modified();
    notifyObserversProcessorNetworkDidAddProcessor(processor);
}

void ProcessorNetwork::removeProcessor(Processor* processor) {
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
    for (int i = 0; i < inports.size(); ++i) {
        outports = inports[i]->getConnectedOutports();
        for (int j = 0; j < outports.size(); ++j) {
            removeConnection(outports[j], inports[i]);
        }
    }

    // Workaround for multiInport shit.
    // The code above should work, but a MultiInport has several Inports in it.
    // So a connecion in made between a Outport and a MultiInport. But when you ask
    // the Outport for it's connected inports you will NOT get the MultiInport but a
    // SingleInport within the MultiInport. This is not the same port as the connection
    // was make to, hence will will not find it, and can not delete it!!!
    // TODO when the MultiInport behave properly, remove this.
    PortConnectionMap connections = portConnections_;
    for (PortConnectionMap::iterator it = connections.begin(); it != connections.end(); ++it) {
        if(it->second->getInport()->getProcessor() == processor ||
           it->second->getOutport()->getProcessor() == processor) {
        
            removeConnection(it->second->getOutport(), it->second->getInport());
        }
    }

    // Remove all links for this processor
    PropertyLinkMap propertyLinks = propertyLinks_;
    for (PropertyLinkMap::iterator it = propertyLinks.begin(); it != propertyLinks.end(); ++it) {
        if (it->second->getSourceProperty()->getOwner()->getProcessor() == processor ||
            it->second->getDestinationProperty()->getOwner()->getProcessor() == processor) {
            removeLink(it->second->getSourceProperty(), it->second->getDestinationProperty());
        }
    }
    

    // remove processor itself
    notifyObserversProcessorNetworkWillRemoveProcessor(processor);
    ProcessorMap::iterator it = processors_.find(processor->getIdentifier());
    if(it != processors_.end()) processors_.erase(it);
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
    ProcessorMap::const_iterator it = processors_.find(identifier);
    if(it!= processors_.end()) return it->second;
    return NULL;
}

std::vector<Processor*> ProcessorNetwork::getProcessors() const {
    ProcessorVector processors;
    for (ProcessorMap::const_iterator it = processors_.begin(); it != processors_.end(); ++it) {
        processors.push_back(it->second);
    }
    return processors;
}


PortConnection* ProcessorNetwork::addConnection(Outport* sourcePort, Inport* destPort) {
    PortConnection* connection = getConnection(sourcePort, destPort);

    if (!connection && sourcePort && destPort && destPort->canConnectTo(sourcePort)) {
        notifyObserversProcessorNetworkWillAddConnection(connection);

        connection = new PortConnection(sourcePort, destPort);
        portConnections_[std::make_pair(sourcePort, destPort)] = connection;
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

/////////////////////////////////////////////////////////////////////////////////////////
//Property Links

PropertyLink* ProcessorNetwork::addLink(Property* sourceProperty, Property* destinationProperty) {
    PropertyLink* link = getLink(sourceProperty, destinationProperty);
    if (!link) {
        link = new PropertyLink(sourceProperty, destinationProperty);
        notifyObserversProcessorNetworkWillAddLink(link);
        propertyLinks_[std::make_pair(sourceProperty,destinationProperty)] = link;
        addToPrimaryCache(link); //add to cache
        modified();
        notifyObserversProcessorNetworkDidAddLink(link);
    }
    return link;
}

void ProcessorNetwork::removeLink(Property* sourceProperty, Property* destinationProperty) {
    PropertyLinkMap::iterator it =
        propertyLinks_.find(std::make_pair(sourceProperty, destinationProperty));
    if (it != propertyLinks_.end()) {
        PropertyLink* link = it->second;
        notifyObserversProcessorNetworkWillRemoveLink(link);
        removeFromPrimaryCache(link);
        propertyLinks_.erase(it);
        modified();
        notifyObserversProcessorNetworkDidRemoveLink(link);
        delete link;
    }
}

bool ProcessorNetwork::isLinked(Property* sourceProperty, Property* destinationProperty) {
    if (getLink(sourceProperty, sourceProperty))
        return true;
    return false;
}

PropertyLink* ProcessorNetwork::getLink(Property* sourceProperty,
                                        Property* destinationProperty) const {
    PropertyLinkMap::const_iterator it =
        propertyLinks_.find(std::make_pair(sourceProperty, destinationProperty));
    if (it != propertyLinks_.end()) {
        return it->second;
    }
    return NULL;
}

std::vector<PropertyLink*> ProcessorNetwork::getLinks() const {
    PropertyLinkVector links;
    for (PropertyLinkMap::const_iterator it = propertyLinks_.begin(); it != propertyLinks_.end();
         ++it) {
        links.push_back(it->second);
    }
    return links;
}

bool ProcessorNetwork::isLinkedBidirectional(Property* startProperty, Property* endProperty) {
    return getLink(startProperty, endProperty) != NULL &&
           getLink(endProperty, startProperty) != NULL;
}

ProcessorNetwork::PropertyLinkVector ProcessorNetwork::getLinksBetweenProcessors(
    Processor* p1, Processor* p2) {
    
    ProcessorLinkMap::iterator it = processorLinksCache_.find(ProcessorPair(p1,p2));
    if (it != processorLinksCache_.end()) {
        return it->second;
    }
    return PropertyLinkVector();
}

void ProcessorNetwork::addToPrimaryCache(PropertyLink* propertyLink) {
    Property* srcProperty = propertyLink->getSourceProperty();
    Property* dstProperty = propertyLink->getDestinationProperty();

    // Update ProcessorLink cache
    Processor* p1 = srcProperty->getOwner()->getProcessor();
    Processor* p2 = dstProperty->getOwner()->getProcessor();
    ProcessorLinkMap::iterator it = processorLinksCache_.find(ProcessorPair(p1,p2));
    if (it != processorLinksCache_.end()) {
        it->second.push_back(propertyLink);
    } else {
        processorLinksCache_[ProcessorPair(p1,p2)].push_back(propertyLink);
    }
    
    // Update primary cache
    if (std::find(propertyLinkPrimaryCache_[srcProperty].begin(),
                  propertyLinkPrimaryCache_[srcProperty].end(),
                  dstProperty) == propertyLinkPrimaryCache_[srcProperty].end()) {

        propertyLinkPrimaryCache_[srcProperty].push_back(dstProperty);
    }

    if (propertyLinkPrimaryCache_[srcProperty].size() == 0) {
        propertyLinkPrimaryCache_.erase(srcProperty);
    }

    clearSecondaryCache();
}

void ProcessorNetwork::removeFromPrimaryCache(PropertyLink* propertyLink) {
    Property* srcProperty = propertyLink->getSourceProperty();
    Property* dstProperty = propertyLink->getDestinationProperty();

    // Update ProcessorLink cache
    Processor* p1 = srcProperty->getOwner()->getProcessor();
    Processor* p2 = dstProperty->getOwner()->getProcessor();

    ProcessorLinkMap::iterator it = processorLinksCache_.find(ProcessorPair(p1, p2));
    if (it != processorLinksCache_.end()) {
        it->second.erase(std::remove(it->second.begin(), it->second.end(), propertyLink),
                         it->second.end());
        if (it->second.size() == 0) {
            processorLinksCache_.erase(it);
        }
    }

    // Update primary cache
    std::vector<Property*>::iterator sIt =
        std::find(propertyLinkPrimaryCache_[srcProperty].begin(),
                  propertyLinkPrimaryCache_[srcProperty].end(), dstProperty);

    if (sIt != propertyLinkPrimaryCache_[srcProperty].end()) {
        propertyLinkPrimaryCache_[srcProperty].erase(sIt);
    }

    if (propertyLinkPrimaryCache_[srcProperty].size() == 0) {
        propertyLinkPrimaryCache_.erase(srcProperty);
    }

    clearSecondaryCache();
}

void ProcessorNetwork::updatePropertyLinkCaches() {
    clearSecondaryCache();
    propertyLinkPrimaryCache_.clear();
    for (PropertyLinkMap::iterator it = propertyLinks_.begin(); it != propertyLinks_.end();
         ++it) {
        addToPrimaryCache(it->second);
    }

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

struct LinkCheck {
    LinkCheck() : linkSettings_(InviwoApplication::getPtr()->getSettingsByType<LinkSettings>()){ }
    bool operator()(const Property *p)const{ return !linkSettings_->isLinkable(p); }
private:
    LinkSettings* linkSettings_;
};

struct AutoLinkCheck {
    AutoLinkCheck(const Property *p, LinkingConditions linkCondition) : property_(p), linkCondition_(linkCondition){}
    bool operator()(const Property *p)const{ return !AutoLinker::canLink(p, property_, linkCondition_); }
private:
    const Property *property_;
    LinkingConditions linkCondition_;
};




struct AutoLinkSort{
    AutoLinkSort(const Property *p){
        pos_ = getPosition(p);
    }

    bool operator()(const Property *a, const Property *b){
        // TODO Figure out which candidate is best.
        // using distance now
        float da = glm::distance(pos_, getPosition(a));
        float db = glm::distance(pos_, getPosition(b));
        return da < db;
    }

private:
    vec2 pos_;
    std::map<const Property *, vec2> cache_;

    vec2 getPosition(const Property *p) {
        std::map<const Property *, vec2>::const_iterator it = cache_.find(p);
        if (it != cache_.end())
            return it->second;
        return cache_[p] = getPosition(p->getOwner()->getProcessor());
    }

    vec2 getPosition(const Processor* processor) {
            ProcessorMetaData* meta =processor->getMetaData<ProcessorMetaData>("ProcessorMetaData");
           if (meta) {
               return static_cast<vec2>(meta->getPosition());
           }
           else {
                LogWarnCustom("getProcessorPosition", "No ProcessorMetaData for added processor found while auto linking");
                return vec2(0, 0);
            }
        return vec2(0, 0);
    }
};

void ProcessorNetwork::autoLinkProcessor(Processor* processor) {
    LinkCheck linkChecker;

    std::vector<Property*> destprops = getPropertiesRecursive(processor);
    destprops.erase(std::remove_if(destprops.begin(), destprops.end(), linkChecker), destprops.end()); //remove properties for which autolinking is disalbed

    if (destprops.size() == 0) {  // no candidates for autolinking in the new processor
        return;
    }

    std::vector<Property*> properties;
    for (ProcessorMap::iterator it = processors_.begin(); it != processors_.end(); ++it) {
        if( it->second != processor) {
            std::vector<Property*> p = getPropertiesRecursive(it->second);
            properties.insert(properties.end(),p.begin(), p.end());
        }
    }
    properties.erase(std::remove_if(properties.begin(), properties.end(), linkChecker), properties.end()); //remove properties for which autolinking is disalbed
    if (properties.size() == 0) {  // no candidates for autolinking in the new processor
        return;
    }


    for(std::vector<Property*>::iterator dit = destprops.begin(); dit!=destprops.end(); ++dit) {
        std::vector<Property*> candidates = properties;
        AutoLinkCheck autoLinkChecker(*dit, LinkMatchingTypeAndId);
        std::remove_if(candidates.begin(), candidates.end(), autoLinkChecker);
        AutoLinkSort sorter(*dit);
        std::sort(candidates.begin(), candidates.end(), sorter);

        if(candidates.size()>0) {
            addLink(candidates[0], *dit);
            addLink(*dit, candidates[0]);
        }        
    }
}

std::vector<Property*> ProcessorNetwork::getPropertiesRecursive(PropertyOwner* owner) {
    std::vector<Property*> properties;
    std::vector<Property*> props = owner->getProperties();
    for(std::vector<Property*>::iterator it = props.begin(); it!=props.end(); ++it) {
        properties.push_back(*it);
        PropertyOwner* o = dynamic_cast<PropertyOwner*>(*it);
        if(o){
            std::vector<Property*> p = getPropertiesRecursive(o);
            properties.insert(properties.end(),p.begin(), p.end());
        }
    }
    return properties;
}

/////////////////////////////////////////////////////////////////////////////////////////

void ProcessorNetwork::clear() {
    std::vector<Processor*> processors = getProcessors();

    lock();

    //Invalidate inports to alert processors that they should stop their calculations.
    // TODO Check if needed? solve in a nicer way... /Peter
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

void ProcessorNetwork::onProcessorIdentifierChange(Processor* processor) {
    for (ProcessorMap::iterator it = processors_.begin(); it != processors_.end(); ++it) {
        if(it->second == processor) {
            processors_.erase(it);
            processors_[processor->getIdentifier()] = processor;
            break;
        }
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

    std::vector<Property*> destinationProperties = getLinkedProperties(modifiedProperty);
    for (size_t i=0; i<destinationProperties.size(); i++) {
        linkEvaluator_->evaluate(modifiedProperty, destinationProperties[i]);
    }

    unlock();
    if (isLinking()) linking_ = false;
}

void ProcessorNetwork::serialize(IvwSerializer& s) const {
    s.serialize("ProcessorNetworkVersion", processorNetworkVersion_);
    s.serialize("Processors", getProcessors(), "Processor");
    s.serialize("Connections", getConnections(), "Connection");
    s.serialize("PropertyLinks", getLinks(), "PropertyLink");
}

void ProcessorNetwork::deserialize(IvwDeserializer& d) throw (Exception) {
    // This will set deserializing_ to true while keepTrueWillAlive is in scope
    // and set it to false no matter how we leave the scope
    KeepTrueWhileInScope keepTrueWillAlive(&deserializing_);

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
        ProcessorVector processors;
        d.deserialize("Processors", processors, "Processor");
        for (int i = 0; i < processors.size(); ++i) {
            if (processors[i]) {
                addProcessor(processors[i]);
            }else{
                LogWarn("Failed deserialization: Processor Nr." << i);
            }
        }
    }
    catch (const SerializationException& exception) {
        clear();
        throw AbortException("DeSerialization exception " + exception.getMessage());
    }
    catch (...) {
        clear();
        throw AbortException("Unknown Exception.");
    }

    //Connections
    try {
        std::vector<PortConnection*> portConnections;
        d.deserialize("Connections", portConnections, "Connection");

        for (size_t i=0; i<portConnections.size(); i++) {
            if (portConnections[i]) {
                Outport* outPort = portConnections[i]->getOutport();
                Inport* inPort = portConnections[i]->getInport();

                if (!(outPort && inPort && addConnection(outPort, inPort))) {
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
        clear();
        throw AbortException("Unknown Exception.");
    }

    //Links
    try {
        std::vector<PropertyLink*> propertyLinks;
        d.deserialize("PropertyLinks", propertyLinks, "PropertyLink");
        
        for (size_t j=0; j<propertyLinks.size(); j++) {
            if (propertyLinks[j]->getSourceProperty() && propertyLinks[j]->getDestinationProperty()) {
                addLink(propertyLinks[j]->getSourceProperty(), propertyLinks[j]->getDestinationProperty());
                delete propertyLinks[j];             
            } else {
                LogWarn("Unable to establish property link Nr: " << j);
            }
        }
    } catch (const SerializationException& exception) {
        throw IgnoreException("DeSerialization Exception " + exception.getMessage());
    } catch (...) {
        clear();
        throw AbortException("Unknown Exception.");
    }

    notifyObserversProcessorNetworkChanged();
}

bool ProcessorNetwork::isDeserializing()const {
    return deserializing_;
}

const int ProcessorNetwork::processorNetworkVersion_ = 3;


ProcessorNetwork::NetworkConverter::NetworkConverter(int from)
    : VersionConverter(), from_(from) {}

bool ProcessorNetwork::NetworkConverter::convert(TxElement* root) {
    switch (from_) {
        case 0:
            traverseNodes(root, &ProcessorNetwork::NetworkConverter::updateProcessorType);
        case 1:
            traverseNodes(root, &ProcessorNetwork::NetworkConverter::updateMetaDataTree);
        case 2:
            traverseNodes(root, &ProcessorNetwork::NetworkConverter::updatePropertType);
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
            node->SetAttribute("type", "org.inviwo." + type);
        }
    }
    if (key == "MetaDataList"){
        node->SetValue("MetaDataMap");
    }
    if (key == "MetaData"){
        node->SetValue("MetaDataItem");
    }
}

void ProcessorNetwork::NetworkConverter::updateMetaDataTree(TxElement* node) {
    std::string key;
    node->GetValue(&key);

    if (key == "MetaDataList") {
        node->SetValue("MetaDataMap");
    }
    if (key == "MetaData") {
        node->SetValue("MetaDataItem");
        node->SetAttribute("key", node->GetAttribute("type"));
    }
}

void ProcessorNetwork::NetworkConverter::updatePropertType(TxElement* node) {
    std::string key;
    node->GetValue(&key);

    std::string renamed[] = {
        "undefined",
        "BoolProperty",
        "AdvancedMaterialProperty",
        "BaseOptionProperty",
        "OptionPropertyFloat",
        "OptionPropertyDouble",
        "OptionPropertyInt",
        "OptionPropertyInt64",
        "OptionPropertyFloatVec2",
        "OptionPropertyFloatVec3",
        "OptionPropertyFloatVec4",
        "OptionPropertyDoubleVec2",
        "OptionPropertyDoubleVec3",
        "OptionPropertyDoubleVec4",
        "OptionPropertyIntVec2",
        "OptionPropertyIntVec3",
        "OptionPropertyIntVec4",
        "OptionPropertyFloatMat2",
        "OptionPropertyFloatMat3",
        "OptionPropertyFloatMat4",
        "OptionPropertyDoubleMat2",
        "OptionPropertyDoubleMat3",
        "OptionPropertyDoubleMat4",
        "ButtonProperty",
        "CameraProperty",
        "CompositeProperty",
        "DirectoryProperty",
        "EventProperty",
        "FileProperty",
        "ImageEditorProperty",
        "FloatMinMaxProperty",
        "DoubleMinMaxProperty",
        "IntMinMaxProperty",
        "FloatProperty",
        "DoubleProperty",
        "IntProperty",
        "Int64Property",
        "FloatVec2Property",
        "FloatVec3Property",
        "FloatVec4Property",
        "DoubleVec2Property",
        "DoubleVec3Property",
        "DoubleVec4Property",
        "IntVec2Property",
        "IntVec3Property",
        "IntVec4Property",
        "FloatMat2Property",
        "FloatMat3Property",
        "FloatMat4Property",
        "DoubleMat2Property",
        "DoubleMat3Property",
        "DoubleMat4Property",
        "SimpleLightingProperty",
        "SimpleRaycastingProperty",
        "StringProperty",
        "TransferFunctionProperty" };

    if (key == "Property") {
        std::string type = node->GetAttributeOrDefault("type", "");
        int size = sizeof(renamed)/sizeof(std::string);
        if(std::find(renamed, renamed+size, type) != renamed+size) {
            node->SetAttribute("type", "org.inviwo." + type);
        }
    }
}





} // namespace
