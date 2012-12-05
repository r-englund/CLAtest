#include "inviwo/core/network/processorlink.h"
#include "inviwo/core/properties/linkevaluator.h"

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

PropertyLink::PropertyLink(Property* srcProperty, Property* destProperty, bool biDirectional) 
              : srcProperty_(srcProperty), dstProperty_(destProperty),
                isBidirectional_(biDirectional){

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

bool ProcessorLink::isLinked(Property* startProperty, Property* endProperty) {
    bool isLinkFound = false;
    for (size_t i=0; i<propertyLinks_.size(); i++) {
        if ( (propertyLinks_[i]->getSourceProperty() == startProperty && propertyLinks_[i]->getDestinationProperty() == endProperty) ||
            (propertyLinks_[i]->getSourceProperty() == endProperty && propertyLinks_[i]->getDestinationProperty() == startProperty) ) {
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

    if ( (startProperty->getOwner() == outProcessor && endProperty->getOwner() == inProcessor) ||
         (startProperty->getOwner() == inProcessor && endProperty->getOwner() == outProcessor) ) {
        propertyLinks_.push_back(new PropertyLink(startProperty, endProperty));
    }  
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
