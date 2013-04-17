#ifndef IWE_ACTION_H
#define IWE_ACTION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

class IVW_CORE_API Action : public IvwSerializable {
public:

    Action();        
    virtual ~Action();

    std::string name() { return actionName_; }
    int action() { return action_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

protected:
    std::string actionName_;
    int action_;
};
} // namespace

#endif // IWE_ACTION_H