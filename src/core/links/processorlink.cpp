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

#include <inviwo/core/links/processorlink.h>

namespace inviwo {

ProcessorLink::ProcessorLink()   
    : sourceProcessor_(0),
      destinationProcessor_(0){}

ProcessorLink::ProcessorLink(Processor* outProecessor, Processor* inProecessor)
    : sourceProcessor_(inProecessor),
      destinationProcessor_(outProecessor){}

ProcessorLink::~ProcessorLink() {}

void ProcessorLink::autoLinkPropertiesByType() {    
    //This is just for testing. Best to use if processors are of same type
    //This links all the properties of source processor to destination processor
    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;
    std::vector<Property*> srcProperties = outProcessor->getProperties();
    Property* dstProperty=0;
    LinkEvaluator leval;
    for (size_t i=0; i<srcProperties.size(); i++) {
        dstProperty = inProcessor->getPropertyByIdentifier(srcProperties[i]->getClassName());
        if (dstProperty) leval.evaluate(srcProperties[i], dstProperty);
    }
}

bool ProcessorLink::isValid() {
    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;

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

void ProcessorLink::evaluate(LinkEvaluator *leval) {
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

std::vector<Property*> ProcessorLink::getSourceProperties() {
    std::vector<Property*> sourceProperties;
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        sourceProperties.push_back(propertyLinks_[i]->getSourceProperty());
    }
    return sourceProperties;
}

bool ProcessorLink::isLinked(Property* startProperty, Property* endProperty) {
    bool isLinkFound = false;
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if ( (propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty) ) {
                isLinkFound = true;
                break;
        }
    }
    return isLinkFound;
}

void ProcessorLink::addPropertyLinks(Property* startProperty, Property* endProperty) {
    //do assertion     
    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;

    if (isLinked(startProperty, endProperty)) return;

    PropertyLink* newLink = 0;

    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
         (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {
            newLink = new PropertyLink(startProperty, endProperty);
    }

    if (newLink) {
        propertyLinks_.push_back(newLink);
    }
}

void ProcessorLink::removeBidirectionalPair(Property* startProperty, Property* endProperty) {

    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;

    //if (!isLinked(startProperty, endProperty)) return;

    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
        (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {  

        PropertyLink* pair = getBidirectionalPair(startProperty, endProperty);

        if (pair) {
            for (size_t i=0; i<propertyLinks_.size(); i++) {
                if ( (propertyLinks_[i]->getSourceProperty() == pair->getSourceProperty() && 
                    propertyLinks_[i]->getDestinationProperty() == pair->getDestinationProperty()) ) {                        
                        PropertyLink* plink = propertyLinks_[i];
                        propertyLinks_.erase(propertyLinks_.begin()+i);
                        delete plink;
                        break;
                }
            }
        }
    }
}

void ProcessorLink::removePropertyLinks(Property* startProperty, Property* endProperty) {
    //do assertion     
    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;

    //if (!isLinked(startProperty, endProperty)) return;

    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
         (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {

            removeBidirectionalPair(startProperty, endProperty);
            PropertyLink* plink = 0;
            for (size_t i=0; i<propertyLinks_.size(); i++) {
                if ( (propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty) )
                    plink = propertyLinks_[i];                    
            }
            removePropertyLink(plink);
    }  
}

void ProcessorLink::removePropertyLink(PropertyLink* plink) {
    if (!plink) {
        LogWarn("Invalid property link requested for removal");
        return;
    }

    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (plink == propertyLinks_[i] ) {
            propertyLinks_.erase(propertyLinks_.begin()+i);
            delete plink;
            break;
        }
    }
}

PropertyLink* ProcessorLink::getPropertyLink(Property* startProperty, Property* endProperty) {
    Processor* outProcessor = destinationProcessor_;
    Processor* inProcessor = sourceProcessor_;

    //if (!isLinked(startProperty, endProperty)) return;
    PropertyLink* plink=0;
    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
        (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {
            for (size_t i=0; i<propertyLinks_.size(); i++) {
                if ( (propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty) ) {
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

std::string ProcessorLink::getLinkInfo() {
    std::string info("");
    if (!propertyLinks_.size()) return info;

    Processor* outProcessor = dynamic_cast<Processor*>(propertyLinks_[0]->getSourceProperty()->getOwner());
    Processor* inProcessor = dynamic_cast<Processor*>(propertyLinks_[0]->getDestinationProperty()->getOwner());  

    std::string outId = outProcessor->getIdentifier();
    std::string inId = inProcessor->getIdentifier();    

    std::vector<PropertyLink*> processedLinks;
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if (std::find(processedLinks.begin(), processedLinks.end(), propertyLinks_[i])==processedLinks.end()) {
            Property* srcProperty = propertyLinks_[i]->getSourceProperty();
            Property* dstProperty = propertyLinks_[i]->getDestinationProperty(); 
            PropertyLink* pairLink = getBidirectionalPair(srcProperty, dstProperty);
            if (pairLink) 
                processedLinks.push_back(pairLink); 
            processedLinks.push_back(propertyLinks_[i]);
            if (!info.empty())
                info+="\n";
            info += outId + ":" + srcProperty->getDisplayName() + " - " + inId + ":" + dstProperty->getDisplayName() ;            
        }
    }

    return info;

}

void ProcessorLink::serialize(IvwSerializer& s) const {
    s.serialize("PropertyLinks", propertyLinks_, "PropertyLink");
}

void ProcessorLink::deserialize(IvwDeserializer& d) {
    propertyLinks_.clear();
    d.deserialize("PropertyLinks", propertyLinks_, "PropertyLink");
    if(propertyLinks_[0]){
        Property* srcProp = propertyLinks_[0]->getSourceProperty();
        if(srcProp)
            destinationProcessor_ = dynamic_cast<Processor*>(srcProp->getOwner());
    
        Property* destProp = propertyLinks_[0]->getDestinationProperty();
        if(destProp)
            sourceProcessor_ = dynamic_cast<Processor*>(destProp->getOwner());
    }
    if(!destinationProcessor_ || !sourceProcessor_){
        LogWarn("Could not deserialize property links.");
        std::vector<PropertyLink*>::iterator it = propertyLinks_.begin();
        while (it != propertyLinks_.end()) {
            delete *it;
            it = propertyLinks_.erase(it);            
        }
    }
}

} // namespace
