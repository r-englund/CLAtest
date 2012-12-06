#ifndef IVW_PROCESSORCONNECTION_H
#define IVW_PROCESSORCONNECTION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"

namespace inviwo {

class PropertyLink : public IvwSerializable {
public :
    PropertyLink();
    PropertyLink(Property* srcProperty, Property* dstProperty, bool biDirectional=false);
    virtual ~PropertyLink();
    
    void setSourceProperty(Property* src) { srcProperty_=src; }
    void setDestinationProperty(Property* dst) { dstProperty_=dst; }
    void setBirectional(bool biDirectional) {isBidirectional_ = biDirectional;}

    Property* getSourceProperty() const{ return srcProperty_; }
    Property* getDestinationProperty() const{ return dstProperty_; }

    bool isBidirectional() {return isBidirectional_;}
    void switchDirection();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    Property* srcProperty_;
    Property* dstProperty_;
    bool isBidirectional_;
};

class ProcessorLink : public IvwSerializable {

public:    

    ProcessorLink();
    ProcessorLink(Processor* outProcessor, Processor* inProcessor);
    virtual ~ProcessorLink();

    Processor* getInProcessor() const{ return inProcessor_.getProcessor(); }
    Processor* getOutProcessor() const{ return outProcessor_.getProcessor(); }

    void autoLinkPropertiesByType();
    void evaluate();
    bool isLinked(Property* startProperty, Property* endProperty);

    void addPropertyLinks(Property* startProperty, Property* endProperty);
    void removePropertyLinks(Property* startProperty, Property* endProperty);
    std::vector<PropertyLink*> getPropertyLinks() {return propertyLinks_;} 

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
   std::vector<PropertyLink*> propertyLinks_;
};

} // namespace

#endif // IVW_PORTCONNECTION_H
