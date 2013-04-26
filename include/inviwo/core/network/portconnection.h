#ifndef IVW_PORTCONNECTION_H
#define IVW_PORTCONNECTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_CORE_API PortConnection : public IvwSerializable {

public:
    class SlimPort : public IvwSerializable {
    public:
        SlimPort();
        SlimPort(Port*);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& s);
        Port* getPort() const;
    private:
        Port* port_;
    };

    class SlimInport : public SlimPort {
    public:
        SlimInport();
        SlimInport(Inport*);
        virtual void deserialize(IvwDeserializer& s);
        Inport* getInport() const;
    private:
        Inport* inport_;
    };

    class SlimOutport : public SlimPort {
    public:
        SlimOutport();
        SlimOutport(Outport*);
        virtual void deserialize(IvwDeserializer& s);
        Outport* getOutport() const;
    private:
        Outport* outport_;
    };

    PortConnection();
    PortConnection(Outport* outport, Inport* inport);
    virtual ~PortConnection();

    Inport* getInport() const { return inport_.getInport(); }
    Outport* getOutport() const { return outport_.getOutport(); }

    bool involvesProcessor(Processor* processor) const {
        return (inport_.getPort()->getProcessor()==processor ||
                outport_.getPort()->getProcessor()==processor);
    }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    SlimInport inport_;
    SlimOutport outport_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
