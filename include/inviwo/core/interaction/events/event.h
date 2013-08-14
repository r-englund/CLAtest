#ifndef IVW_EVENT_H
#define IVW_EVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

class IVW_CORE_API Event : public IvwSerializable {
public:
    
    Event();
    virtual ~Event();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_EVENT_H