#ifndef IWE_ACTION_H
#define IWE_ACTION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

    class IVW_CORE_API Action {
    public:

        Action();        
        virtual ~Action();

        std::string name() { return actionName_; }
        int action() { return action_; }

    protected:
        std::string actionName_;
        int action_;
    };
} // namespace

#endif // IWE_ACTION_H