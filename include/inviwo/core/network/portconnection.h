#ifndef IVW_PORTCONNECTION_H
#define IVW_PORTCONNECTION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/ports/port.h>
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
        Port* _port;
    };

    PortConnection();
    PortConnection(Port* outport, Port* inport);
    virtual ~PortConnection();

    Port* getInport() const{ return inport_.getPort(); }
    Port* getOutport() const{ return outport_.getPort(); }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    


    SlimPort inport_;
    SlimPort outport_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
