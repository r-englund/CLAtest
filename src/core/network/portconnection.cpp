#include "inviwo/core/network/portconnection.h"

namespace inviwo {

PortConnection::SlimPort::SlimPort() {
}
PortConnection::SlimPort::SlimPort(Port* p):_port(p) {
}

void PortConnection::SlimPort::serialize(IvwSerializer& s) const {
    s.serialize("identifier", _port->getIdentifier(), true);
    s.serialize("Processor", _port->getProcessor());
}

void PortConnection::SlimPort::deserialize(IvwDeserializer& s) {
   std::string name;
   Processor* processor=0;   

   s.deserialize("identifier", name, true);
   s.deserialize("Processor", processor);   

   if(processor) {
       _port = processor->getPort(name);       
   }
}

Port* PortConnection::SlimPort::getPort() const{
    return _port;
}

PortConnection::PortConnection()   
    : outport_(0),
      inport_(0) {}

PortConnection::PortConnection(Port* outport, Port* inport)
    : outport_(outport),
      inport_(inport) {}

PortConnection::~PortConnection() {}

void PortConnection::serialize(IvwSerializer& s) const {
    s.serialize("connection", getInport()->getIdentifier(),true);
    s.serialize("OutPort", outport_);
    s.serialize("InPort", inport_);
}

void PortConnection::deserialize(IvwDeserializer& d) {
    std::string portType;
    d.deserialize("connection", portType, true);
    d.deserialize("OutPort", outport_);
    d.deserialize("InPort", inport_);
}

} // namespace
