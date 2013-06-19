#include <inviwo/core/network/portconnection.h>

namespace inviwo {

PortConnection::PortConnection()   
    : inport_(0),
      outport_(0){}

PortConnection::PortConnection(Outport* outport, Inport* inport)
    : inport_(inport),
      outport_(outport){}

PortConnection::~PortConnection() {}

void PortConnection::serialize(IvwSerializer& s) const {  
    s.serialize("OutPort", *getOutport());
    s.serialize("InPort", *getInport());
}

void PortConnection::deserialize(IvwDeserializer& d) {    
    Outport outport("");
    d.deserialize("OutPort", outport);
    Processor* outPortProcessor = outport.getProcessor();
    ivwAssert(outPortProcessor!=0, "Unable to load processor.")
    
    if (outPortProcessor)
        outport_ = dynamic_cast<Outport*>(outPortProcessor->getPort(outport.getIdentifier()));
    
    Inport inport("");
    d.deserialize("InPort", inport);
    Processor* inPortProcessor = inport.getProcessor();
    ivwAssert(outPortProcessor!=0, "Unable to load processor.")

    if (inPortProcessor)
        inport_ = dynamic_cast<Inport*>(inPortProcessor->getPort(inport.getIdentifier()));
}

} // namespace
