#ifndef IVW_PROCESSORCONNECTION_H
#define IVW_PROCESSORCONNECTION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

class ProcessorLink : public IvwSerializable {

public:
    ProcessorLink();
    ProcessorLink(Processor* outport, Processor* inport);
    virtual ~ProcessorLink();

    Processor* getInProcessor() const{ return inProcessor_; }
    Processor* getOutProcessor() const{ return outProcessor_; }

    void autoLinkPropertiesByType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    Processor* inProcessor_;
    Processor* outProcessor_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
