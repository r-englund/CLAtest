#ifndef IVW_PICKINGMANAGER_H
#define IVW_PICKINGMANAGER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {
/** \class PickingManager 
* Manager for picking objects. 
*/

class IVW_CORE_API PickingManager {

public:
    static PickingManager* instance() {
        static PickingManager instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    ~PickingManager();
   
private:
    PickingManager() {};
    PickingManager(PickingManager const&) {};
    void operator=(PickingManager const&) {};

    std::vector<PickingObject*> pickingObjects_;
};

} // namespace

#endif // IVW_PICKINGMANAGER_H