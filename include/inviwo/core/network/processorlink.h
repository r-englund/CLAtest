#ifndef IVW_PROCESSORCONNECTION_H
#define IVW_PROCESSORCONNECTION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

class ProcessorLink : public IvwSerializable {

public:    

    ProcessorLink();
    ProcessorLink(Processor* outProcessor, Processor* inProcessor);
    virtual ~ProcessorLink();

    Processor* getInProcessor() const{ return inProcessor_.getProcessor(); }
    Processor* getOutProcessor() const{ return outProcessor_.getProcessor(); }

    void autoLinkPropertiesByType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    //TODO: This is just for serialization/deserialization
    //This is temporary fix. Serialization needs improvement.
    class SlimProcessor : public IvwSerializable {
    public:
        SlimProcessor();
        SlimProcessor(Processor*);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& s);
        Processor* getProcessor() const;
    private:
        Processor* processor_;
    };

   SlimProcessor inProcessor_;
   SlimProcessor outProcessor_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
