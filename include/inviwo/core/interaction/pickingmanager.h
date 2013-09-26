#ifndef IVW_PICKINGMANAGER_H
#define IVW_PICKINGMANAGER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {
/** \class PickingManager 
* Manager for picking objects. 
*/

class FindPickingObject
{
public:
    FindPickingObject(const vec3& c) : color_(c) {}

    bool operator()(PickingObject* obj){ 
        return obj->getPickingColor() == color_; 
    }
private:
    vec3 color_;
};

class IVW_CORE_API PickingManager {

public:
    static PickingManager* instance() {
        static PickingManager instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    ~PickingManager();

    template <typename T>
    const PickingObject* registerPickingCallback(T* o, void (T::*m)()){
        PickingObject* pickObj = generatePickingObject(pickingObjects_.size());
        pickObj->getCallbackContainer()->addMemberFunction(o,m);
        pickingObjects_.push_back(pickObj);
        return pickObj;
    }

    void handlePickedColor(const vec3&);
   
protected:
    PickingManager() {};
    PickingManager(PickingManager const&) {};
    void operator=(PickingManager const&) {};

    PickingObject* generatePickingObject(size_t);

private:
    std::vector<PickingObject*> pickingObjects_;
};

} // namespace

#endif // IVW_PICKINGMANAGER_H