#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/util/colorconversion.h>

namespace inviwo {

PickingManager::~PickingManager() {
    for(std::vector<PickingObject*>::iterator it = pickingObjects_.begin(); it != pickingObjects_.end(); it++) {
        delete *it;
    }
}

void PickingManager::handlePickedColor(const vec3& c){
    std::vector<PickingObject*>::iterator it = std::find_if(pickingObjects_.begin(), pickingObjects_.end(), FindPickingObject(c));
    if (it != pickingObjects_.end()){
        LogInfo("Found Picking Object with ID : (" << (*it)->getPickingId() << ")");
        (*it)->picked();
    }
}

PickingObject* PickingManager::generatePickingObject(size_t id){
    float idF = static_cast<float>(id);

    // Hue /Saturation / Value
    // Hue is based on Golden Ratio for unique and distinct color differences.
    vec3 hsv = vec3(idF * M_PI - floor(idF * M_PI), 0.5f, 0.95f);
    vec3 rgb = hsv2rgb(hsv);

    return new PickingObject(id, rgb);
}

} // namespace
