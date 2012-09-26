#include "inviwo/core/network/portconnection.h"

namespace inviwo {

PortConnection::PortConnection() {}

PortConnection::PortConnection(Port* outport, Port* inport)
    : outport_(outport),
      inport_(inport) {}

PortConnection::~PortConnection() {}

void PortConnection::serialize(IvwSerializeBase& /*s*/) const {}
void PortConnection::deserialize(IvwSerializeBase& /*s*/) {}

} // namespace
