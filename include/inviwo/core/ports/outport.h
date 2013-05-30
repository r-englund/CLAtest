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

    //TODO: Temporary fix. Remove this to make Outport abstract class
    virtual void initialize() {}
    virtual void deinitialize() {}

    bool isConnected() const;
    bool isConnectedTo(Inport* port) const;

    bool isReady() const { ivwDeprecatedMethod("isConnected()"); return isConnected(); }

    std::vector<Inport*> getConnectedInports() const { return connectedInports_; }

    void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);
    std::vector<Processor*> getDirectSuccessors();

protected:
    void connectTo(Inport* port);
    void disconnectFrom(Inport* port);
    
    template <typename T>
    void getSuccessorsUsingPortType(std::vector<Processor*>&);

private:
    std::vector<Inport*> connectedInports_;
};

} // namespace

#endif // IVW_OUTPORT_H