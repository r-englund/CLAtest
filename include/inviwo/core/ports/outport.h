#ifndef IVW_OUTPORT_H
#define IVW_OUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

class Inport;

class IVW_CORE_API Outport : public Port {

friend class Inport;

public:
    Outport(std::string identifier);
    virtual ~Outport();

    bool isConnected() const;
    bool isConnectedTo(Inport* port) const;

    void invalidate();

protected:
    void connectTo(Inport* port);
    void disconnectFrom(Inport* port);

private:
    std::vector<Inport*> connectedInports_;
};

} // namespace

#endif // IVW_OUTPORT_H