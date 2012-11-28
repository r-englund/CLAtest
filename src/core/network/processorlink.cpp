#include "inviwo/core/network/processorlink.h"

namespace inviwo {

ProcessorLink::ProcessorLink()   
    : outProcessor_(0),
      inProcessor_(0) {}

ProcessorLink::ProcessorLink(Processor* outProecessor, Processor* inProecessor)
    : outProcessor_(outProecessor),
      inProcessor_(inProecessor) {}

ProcessorLink::~ProcessorLink() {}

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
