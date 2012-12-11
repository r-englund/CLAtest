#ifndef IVW_PROCESSORCONNECTION_H
#define IVW_PROCESSORCONNECTION_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"
#include "inviwo/core/properties/linkevaluator.h"

namespace inviwo {

class PropertyLink : public IvwSerializable {
public :
    PropertyLink();
    PropertyLink(Property* srcProperty, Property* dstProperty);
    virtual ~PropertyLink();
    
    void setSourceProperty(Property* src) { srcProperty_=src; }
    void setDestinationProperty(Property* dst) { dstProperty_=dst; } 

    Property* getSourceProperty() const{ return srcProperty_; }
    Property* getDestinationProperty() const{ return dstProperty_; }  

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

    void switchDirection();

private:
    Property* srcProperty_;
    Property* dstProperty_;
};

class ProcessorLink : public IvwSerializable {

public:    

    ProcessorLink();
    ProcessorLink(Processor* outProcessor, Processor* inProcessor);
    virtual ~ProcessorLink();

    Processor* getInProcessor() const{ return inProcessor_.getProcessor(); }
    Processor* getOutProcessor() const{ return outProcessor_.getProcessor(); }

    void autoLinkPropertiesByType();
    void evaluate(LinkEvaluator *leval);
    bool isLinked(Property* startProperty, Property* endProperty);
    bool isValid();
    std::vector<Property*> getSourceProperties();

    void addPropertyLinks(Property* startProperty, Property* endProperty);
    void removePropertyLinks(Property* startProperty, Property* endProperty);
    void removeBidirectionalPair(Property* startProperty, Property* endProperty);
    std::vector<PropertyLink*> getPropertyLinks() {return propertyLinks_;} 
    PropertyLink* getPropertyLink(Property* startProperty, Property* endProperty);
    PropertyLink* getBidirectionalPair(PropertyLink* propertyLink);
    PropertyLink* getBidirectionalPair(Property* startProperty, Property* endProperty);

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
