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

    //FIXME: Temporary fix. Remove this to make Inport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}

    virtual bool canConnectTo(Port* port) const { return false; };
    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    bool isConnected() const;
    bool isConnectedTo(Outport* outport) const;

    bool isReady() const { ivwDeprecatedMethod("isConnected()"); return isConnected(); }

    Outport* getConnectedOutport() const { return connectedOutport_; }

    void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    virtual std::vector<Processor*> getPredecessors();

protected:
    template <typename T>
    void getPredecessorsUsingPortType(std::vector<Processor*>&);

private:
    Outport* connectedOutport_;
};

} // namespace

#endif // IVW_INPORT_H