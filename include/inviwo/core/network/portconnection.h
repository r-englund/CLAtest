#ifndef IVW_PORTCONNECTION_H
#define IVW_PORTCONNECTION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/port.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

class PortConnection : public Serializable {

public:
    PortConnection();
    PortConnection(Port* outport, Port* inport);
    virtual ~PortConnection();

    Port* getInport() { return inport_; }
    Port* getOutport() { return outport_; }

    virtual void serialize(XmlSerializer& s) const;
    virtual void deserialize(XmlDeserializer& s);

private:
    Port* inport_;
    Port* outport_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
