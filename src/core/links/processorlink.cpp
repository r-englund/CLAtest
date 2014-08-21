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

#include <inviwo/core/links/processorlink.h>

namespace inviwo {

ProcessorLink::ProcessorLink()
    : sourceProcessor_(NULL),
      destinationProcessor_(NULL) {}

ProcessorLink::ProcessorLink(PropertyOwner* inProecessor, PropertyOwner* outProecessor)
    : sourceProcessor_(inProecessor),
      destinationProcessor_(outProecessor) {}

ProcessorLink::~ProcessorLink() {}

void ProcessorLink::autoLinkPropertiesByType() {
    //This is just for testing. Best to use if processors are of same type
    //This links all the properties of source processor to destination processor
    PropertyOwner* outProcessor = destinationProcessor_;
    PropertyOwner* inProcessor = sourceProcessor_;
    std::vector<Property*> srcProperties = outProcessor->getProperties();
    Property* dstProperty=0;
    LinkEvaluator leval;

    for (size_t i=0; i<srcProperties.size(); i++) {
        dstProperty = inProcessor->getPropertyByIdentifier(srcProperties[i]->getClassName());

        if (dstProperty) leval.evaluate(srcProperties[i], dstProperty);
    }
}

bool ProcessorLink::isValid() {
    PropertyOwner* outProcessor = destinationProcessor_;
    PropertyOwner* inProcessor = sourceProcessor_;
    bool outProcessorInValid = false;
    bool inProcessorInValid = false;
    std::vector<Property*> outProperties = outProcessor->getProperties();

    for (size_t i=0; i<outProperties.size(); i++) {
        outProcessorInValid = outProperties[i]->isPropertyModified();

        if (outProcessorInValid) break;
    }

    std::vector<Property*> inProperties = inProcessor->getProperties();

    for (size_t i=0; i<inProperties.size(); i++) {
        inProcessorInValid = inProperties[i]->isPropertyModified();

        if (inProcessorInValid) break;
    }

    if (outProcessorInValid || inProcessorInValid)
        return false;

    return true;
}

void ProcessorLink::evaluate(LinkEvaluator* leval) {
    //Evaluate only if processors/properties are invalid
    if (isValid()) return;

    Property* startProperty;
    Property* endProperty;
    //pre fetch the validity flags
    std::vector<PropertyLink*> propertyLinks;

    for (size_t i=0; i<propertyLinks_.size(); i++) {
        startProperty = propertyLinks_[i]->getSourceProperty();

        if (startProperty->isPropertyModified())
            propertyLinks.push_back(propertyLinks_[i]);
    }

    for (size_t i=0; i<propertyLinks.size(); i++) {
        startProperty = propertyLinks[i]->getSourceProperty();
        endProperty = propertyLinks[i]->getDestinationProperty();

        //is change due to property modification?
        if (startProperty->isPropertyModified())
            leval->evaluate(startProperty, endProperty);
    }
}

bool ProcessorLink::isLinked(Property* startProperty, Property* endProperty) {
    bool isLinkFound = false;

    for (size_t i = 0; i < propertyLinks_.size(); i++) {
        if ((propertyLinks_[i]->getSourceProperty() == startProperty &&
             propertyLinks_[i]->getDestinationProperty() == endProperty)) {
            isLinkFound = true;
            break;
        }
    }

    return isLinkFound;
}

bool ProcessorLink::isPropertySource(Property* property)  {
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getSourceProperty()==property)
            return true;
    }

    return false;
}

bool ProcessorLink::isPropertyDestination(Property* property)  {    
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getDestinationProperty()==property)
            return true;
    }
    return false;
}


bool ProcessorLink::involvesPropertyOwner(PropertyOwner* processor) {
    return (sourceProcessor_==processor || destinationProcessor_==processor);
}

bool ProcessorLink::involvesProperty(Property* property)  {
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getSourceProperty()==property || propertyLinks_[i]->getDestinationProperty()==property)
            return true;
    }

    return false;
}

void ProcessorLink::addPropertyLinks(PropertyLink* propertyLink) {
    if (std::find(propertyLinks_.begin(), propertyLinks_.end(), propertyLink)==propertyLinks_.end())
        propertyLinks_.push_back(propertyLink);
}

void ProcessorLink::removePropertyLinks(Property* startProperty, Property* endProperty) {
    PropertyLink* plink = 0;

    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if ((propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty)) {
            plink = propertyLinks_[i];
            propertyLinks_.erase(propertyLinks_.begin()+i);
            break;
        }
    }
    
}

void ProcessorLink::removeAllPropertyLinks() {
    propertyLinks_.clear();
}

PropertyLink* ProcessorLink::getPropertyLink(Property* startProperty, Property* endProperty) {
    PropertyOwner* outProcessor = destinationProcessor_;
    PropertyOwner* inProcessor = sourceProcessor_;
    //if (!isLinked(startProperty, endProperty)) return;
    PropertyLink* plink=0;

    if ((startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
        (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor)) {
        for (size_t i=0; i<propertyLinks_.size(); i++) {
            if ((propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty)) {
                plink = propertyLinks_[i];
                break;
            }
        }
    }

    return plink;
}

PropertyLink* ProcessorLink::getBidirectionalPair(Property* startProperty, Property* endProperty) {
    PropertyLink* link = getPropertyLink(startProperty, endProperty);

    if (!link) return 0;

    return getBidirectionalPair(link);
}

PropertyLink* ProcessorLink::getBidirectionalPair(PropertyLink* propertyLink) {
    return getPropertyLink(propertyLink->getDestinationProperty(), propertyLink->getSourceProperty());
}

void ProcessorLink::setSourceModified() {    
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor() == sourceProcessor_) {
            propertyLinks_[i]->getSourceProperty()->propertyModified();
        }
    }
}

void ProcessorLink::setDestinationModified() {
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor() == destinationProcessor_) {
            propertyLinks_[i]->getDestinationProperty()->propertyModified();
        }
    }
}

void ProcessorLink::setModifiedByPropertyOwner(PropertyOwner *processor) {
    if (processor == sourceProcessor_ ) setSourceModified();
    else if (processor == destinationProcessor_ ) setDestinationModified();
    return;
}


std::string ProcessorLink::getLinkInfo() {
    std::string info("");

    if (!propertyLinks_.size()) return info;

    Processor* outProcessor = dynamic_cast<Processor*>(propertyLinks_[0]->getSourceProperty()->getOwner()->getProcessor());
    Processor* inProcessor = dynamic_cast<Processor*>(propertyLinks_[0]->getDestinationProperty()->getOwner()->getProcessor());

    if (!outProcessor || !inProcessor) {
        LogWarn("Invalid processor links");
        return info;
    }

    
    std::vector<PropertyLink*> processedLinks;

    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (std::find(processedLinks.begin(), processedLinks.end(), propertyLinks_[i])==processedLinks.end()) {

            Processor* outProcessor = dynamic_cast<Processor*>(propertyLinks_[i]->getSourceProperty()->getOwner()->getProcessor());
            Processor* inProcessor = dynamic_cast<Processor*>(propertyLinks_[i]->getDestinationProperty()->getOwner()->getProcessor());

            std::string outId = outProcessor->getIdentifier();
            std::string inId = inProcessor->getIdentifier();

            Property* srcProperty = propertyLinks_[i]->getSourceProperty();
            Property* dstProperty = propertyLinks_[i]->getDestinationProperty();
            /*
            PropertyLink* pairLink = getBidirectionalPair(srcProperty, dstProperty);
            if (pairLink)
                processedLinks.push_back(pairLink);
            */

            processedLinks.push_back(propertyLinks_[i]);

            if (!info.empty())
                info+="\n";

            info += outId + ":" + srcProperty->getDisplayName() + " - " + inId + ":" + dstProperty->getDisplayName() ;
        }
    }

    return info;
}

void ProcessorLink::serialize(IvwSerializer& s) const {
    s.serialize("SourceProcessor", sourceProcessor_);
    s.serialize("DestinationProcessor", destinationProcessor_);
    s.serialize("PropertyLinks", propertyLinks_, "PropertyLink");
    LogWarn("ProcessorLink deprecated")
}

void ProcessorLink::deserialize(IvwDeserializer& d) {
    while (!propertyLinks_.empty()) {
        //delete propertyLinks_.back();
        propertyLinks_.pop_back();
    }

    LogWarn("ProcessorLink deprecated")
    //d.deserialize("SourceProcessor", sourceProcessor_);
    //d.deserialize("DestinationProcessor", destinationProcessor_);
    sourceProcessor_ = 0;
    destinationProcessor_ = 0;

    propertyLinks_.clear();
    d.deserialize("PropertyLinks", propertyLinks_, "PropertyLink");

    if (!sourceProcessor_) {
        if (propertyLinks_[0]) {
            Property* srcProp = propertyLinks_[0]->getSourceProperty();
            if (srcProp) sourceProcessor_ = srcProp->getOwner();
        }
    }

    if (!destinationProcessor_) {
        if (propertyLinks_[0]) {
            Property* destProp = propertyLinks_[0]->getDestinationProperty();
            if (destProp) destinationProcessor_ = destProp->getOwner();
        }
    }

    if (!destinationProcessor_ || !sourceProcessor_) {
        LogWarn("Could not deserialize property links.");
        std::vector<PropertyLink*>::iterator it = propertyLinks_.begin();

        while (it != propertyLinks_.end()) {
            delete *it;
            it = propertyLinks_.erase(it);
        }
    }
}

} // namespace

