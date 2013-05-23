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
    outport_ = dynamic_cast<Outport*>(outport.getProcessor()->getPort(outport.getIdentifier()));
    
    Inport inport("");    
    d.deserialize("InPort", inport);
    inport_ = dynamic_cast<Inport*>(inport.getProcessor()->getPort(inport.getIdentifier()));
}

} // namespace
