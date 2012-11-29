#include "inviwo/core/network/processorlink.h"
#include "inviwo/core/properties/linkevaluator.h"

namespace inviwo {

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
    std::vector<Property*> srcProperties = outProcessor_->getProperties();
    Property* dstProperty=0;
    LinkEvaluator leval;
    for (size_t i=0; i<srcProperties.size(); i++) {
        dstProperty = inProcessor_->getPropertyByIdentifier(srcProperties[i]->getIdentifier());
        if (dstProperty) leval.evaluate(srcProperties[i], dstProperty);
    }
}

void ProcessorLink::serialize(IvwSerializer& s) const {
    s.serialize("processor_link", getInProcessor()->getClassName(), true);
    s.serialize("OutProcessor", outProcessor_);
    s.serialize("InProcessor", inProcessor_);
}

void ProcessorLink::deserialize(IvwDeserializer& d) {
    std::string processorType;
    d.deserialize("processor_link", processorType, true);
    d.deserialize("OutProcessor", outProcessor_);
    d.deserialize("InProcessor", inProcessor_);
}

} // namespace
