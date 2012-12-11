#include "inviwo/core/network/processorlink.h"

namespace inviwo {


ProcessorLink::SlimProcessor::SlimProcessor() {
}

ProcessorLink::SlimProcessor::SlimProcessor(Processor* p):processor_(p) {
}

void ProcessorLink::SlimProcessor::serialize(IvwSerializer& s) const {
    s.serialize("identifier", "", true);
    s.serialize("Processor", processor_);
}

void ProcessorLink::SlimProcessor::deserialize(IvwDeserializer& s) {
    std::string str;
    s.deserialize("identifier", str, true);
    s.deserialize("Processor", processor_);
}

Processor* ProcessorLink::SlimProcessor::getProcessor() const{
    return processor_;
}

PropertyLink::PropertyLink() 
    : srcProperty_(0), 
      dstProperty_(0) {
    
}

PropertyLink::~PropertyLink() {}

PropertyLink::PropertyLink(Property* srcProperty, Property* destProperty) 
              : srcProperty_(srcProperty), dstProperty_(destProperty) {
}

void PropertyLink::switchDirection() {
    Property* tempProperty = srcProperty_;
    srcProperty_ = dstProperty_;
    dstProperty_ = tempProperty;
}

void PropertyLink::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void PropertyLink::deserialize(IvwDeserializer& s) {
    IVW_UNUSED_PARAM(s);
}


ProcessorLink::ProcessorLink()   
    : outProcessor_(0),
      inProcessor_(0) {}

ProcessorLink::ProcessorLink(Processor* outProecessor, Processor* inProecessor)
    : outProcessor_(outProecessor),
      inProcessor_(inProecessor) {}

ProcessorLink::~ProcessorLink() {}

void ProcessorLink::autoLinkPropertiesByType() {    
    //This is just for testing. Best to use if processors are of same type
    //This links all the properties of source processor to destination processor
    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();
    std::vector<Property*> srcProperties = outProcessor->getProperties();
    Property* dstProperty=0;
    LinkEvaluator leval;
    for (size_t i=0; i<srcProperties.size(); i++) {
        dstProperty = inProcessor->getPropertyByIdentifier(srcProperties[i]->getIdentifier());
        if (dstProperty) leval.evaluate(srcProperties[i], dstProperty);
    }
}

bool ProcessorLink::isValid() {
    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();

    if (outProcessor->isValid() && inProcessor->isValid())
        return true;

    return false;
}

void ProcessorLink::evaluate(LinkEvaluator *leval) {
    //Evaluate only if processors/properties are invalid
    if (isValid()) return; 

    Property* startProperty;
    Property* endProperty;

    for (size_t i=0; i<propertyLinks_.size(); i++) {
        startProperty = propertyLinks_[i]->getSourceProperty();
        endProperty = propertyLinks_[i]->getDestinationProperty();
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
    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();

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

    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();

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
    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();

    //if (!isLinked(startProperty, endProperty)) return;

    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
         (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {

            removeBidirectionalPair(startProperty, endProperty);

            for (size_t i=0; i<propertyLinks_.size(); i++) {
                if ( (propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty) ) {                        
                    PropertyLink* plink = propertyLinks_[i];
                    propertyLinks_.erase(propertyLinks_.begin()+i);
                    delete plink;
                    break;
                }
            }
    }  
}

PropertyLink* ProcessorLink::getPropertyLink(Property* startProperty, Property* endProperty) {
    Processor* outProcessor = outProcessor_.getProcessor();
    Processor* inProcessor = inProcessor_.getProcessor();

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

void ProcessorLink::serialize(IvwSerializer& s) const {
    s.serialize("link", getInProcessor()->getClassName(), true);
    s.serialize("OutProcessor", outProcessor_);
    s.serialize("InProcessor", inProcessor_);
}

void ProcessorLink::deserialize(IvwDeserializer& d) {
    std::string processorType;
    d.deserialize("link", processorType, true);
    d.deserialize("OutProcessor", outProcessor_);
    d.deserialize("InProcessor", inProcessor_);
}

} // namespace
