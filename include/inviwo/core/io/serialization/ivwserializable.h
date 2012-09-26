#ifndef IVW_SERIALIZABLE_H
#define IVW_SERIALIZABLE_H

namespace inviwo {

class IvwSerializer;
class IvwDeSerializer;
class IvwSerializeBase;

class IvwSerializable{
public:
    virtual ~IvwSerializable(){}
    virtual void serialize(IvwSerializeBase &s) const=0;
    virtual void deserialize(IvwSerializeBase &d)=0;
};

} //namespace
#endif