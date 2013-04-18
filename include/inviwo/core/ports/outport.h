#ifndef IVW_OUTPORT_H
#define IVW_OUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/inport.h>

namespace inviwo {

class Inport;
class Processor;

class IVW_CORE_API Outport {

friend class Processor;

public:
    Outport(std::string identifier);
    virtual ~Outport();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

    virtual uvec3 getColorCode() const { return uvec3(128,128,128); }

    virtual void connectTo(Inport* port);
    virtual void disconnectFrom(Inport* port);

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const {return identifier_; }

    bool isOutport() const { return true; }
    bool isInport() const { return false; }
    bool isConnected() const { return !(connectedInports_.empty()); }
    bool isConnectedTo(Inport* port) const {
        return !(std::find(connectedInports_.begin(),connectedInports_.end(),port)==connectedInports_.end());
    }

    void invalidate();

protected:
    std::string identifier_;

    void setIdentifier(const std::string& name);
    void setProcessor(Processor* processor) { processor_ = processor; }

private:
    Processor* processor_;
    std::vector<Inport*> connectedInports_;

    //virtual T* getData();
    //void setData(T* data);
    bool hasData() const;

protected:
    //T* data_;
};

} // namespace

#endif // IVW_OUTPORT_H