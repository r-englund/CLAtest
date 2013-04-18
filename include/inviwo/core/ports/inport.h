#ifndef IVW_INPORT_H
#define IVW_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/outport.h>

namespace inviwo {

class Outport;
class Processor;

class IVW_CORE_API Inport {

friend class Processor;

public:
    Inport(std::string identifier);
    virtual ~Inport();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

    virtual uvec3 getColorCode() const { return uvec3(128,128,128); }

    virtual void connectTo(Outport* outport);
    virtual void disconnectFrom(Outport* outport);

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const {return identifier_; }

    bool isOutport() const { return false; }
    bool isInport() const { return true; }

    bool isConnected() const { return (connectedOutport_!=0); }
    bool isConnectedTo(Outport* outport) const {
        return connectedOutport_==outport;
    }

    void invalidate();


protected:
    std::string identifier_;

    void setIdentifier(const std::string& name);
    void setProcessor(Processor* processor) { processor_ = processor; }

private:
    Processor* processor_;
    Outport* connectedOutport_;

    //virtual const T* getData() const;
    bool hasData() const;

    //T* data_;
};

} // namespace

#endif // IVW_INPORT_H