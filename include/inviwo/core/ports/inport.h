#ifndef IVW_INPORT_H
#define IVW_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class Outport;

class IVW_CORE_API Inport : public Port {

public:
    Inport(std::string identifier);
    virtual ~Inport();

    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    bool isConnected() const;
    bool isConnectedTo(Outport* outport) const;

    void invalidate();

private:
    Outport* connectedOutport_;
};

} // namespace

#endif // IVW_INPORT_H