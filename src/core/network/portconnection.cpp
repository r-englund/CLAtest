#include <inviwo/core/network/portconnection.h>

namespace inviwo {

PortConnection::SlimPort::SlimPort():port_(NULL) {}
PortConnection::SlimPort::SlimPort(Port* p):port_(p) {}

void PortConnection::SlimPort::serialize(IvwSerializer& s) const {
    s.serialize("identifier", port_->getIdentifier(), true);
    s.serialize("Processor", port_->getProcessor());
}

void PortConnection::SlimPort::deserialize(IvwDeserializer& s) {
   std::string name;
   Processor* processor=0;   

   s.deserialize("identifier", name, true);
   s.deserialize("Processor", processor);   

   if(processor) {
       port_ = processor->getPort(name);       
   }
}

Port* PortConnection::SlimPort::getPort() const{
    return port_;
}

PortConnection::SlimInport::SlimInport():SlimPort(),inport_(NULL) {}
PortConnection::SlimInport::SlimInport(Inport* p):SlimPort(p),inport_(p) {}

void PortConnection::SlimInport::deserialize(IvwDeserializer& s) {
    SlimPort::deserialize(s);
    inport_ = dynamic_cast<Inport*>(getPort());
}

Inport* PortConnection::SlimInport::getInport() const{
    return inport_;
}

PortConnection::SlimOutport::SlimOutport():SlimPort(),outport_(NULL) {}
PortConnection::SlimOutport::SlimOutport(Outport* p):SlimPort(p),outport_(p) {}

void PortConnection::SlimOutport::deserialize(IvwDeserializer& s) {
    SlimPort::deserialize(s);
    outport_ = dynamic_cast<Outport*>(getPort());
}

Outport* PortConnection::SlimOutport::getOutport() const{
    return outport_;
}

PortConnection::PortConnection()   
    : inport_(0),
      outport_(0){}

PortConnection::PortConnection(Outport* outport, Inport* inport)
    : inport_(inport),
      outport_(outport){}

PortConnection::~PortConnection() {}

void PortConnection::serialize(IvwSerializer& s) const {
    s.serialize("connection", getInport()->getIdentifier(), true);
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
