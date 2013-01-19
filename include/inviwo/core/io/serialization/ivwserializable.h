#ifndef IVW_SERIALIZABLE_H
#define IVW_SERIALIZABLE_H

#include "inviwo/core/inviwocoredefine.h"

namespace inviwo {

class IvwSerializer;
class IvwDeserializer;
class IvwSerializeBase;

class IVW_CORE_API IvwSerializable{
public:
    virtual ~IvwSerializable(){}
    virtual void serialize(IvwSerializer &s) const=0;
    virtual void deserialize(IvwDeserializer &d)=0;
};

} //namespace
#endif