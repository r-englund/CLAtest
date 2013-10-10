#include <inviwo/core/interaction/pickingmanager.h>
#include <inviwo/core/util/colorconversion.h>

namespace inviwo {

PickingManager::~PickingManager() {
    for(std::vector<PickingObject*>::iterator it = pickingObjects_.begin(); it != pickingObjects_.end(); it++) {
        delete *it;
    }
}

void PickingManager::handlePickedColor(const DataVec3UINT8::type& c){
    std::vector<PickingObject*>::iterator it = std::find_if(pickingObjects_.begin(), pickingObjects_.end(), FindPickingObject(c));
    if (it != pickingObjects_.end()){
        (*it)->picked();
    }
}

PickingObject* PickingManager::generatePickingObject(size_t id){
    float idF = static_cast<float>(id);

    // Hue /Saturation / Value
    // Hue is based on Golden Ratio for unique and distinct color differences.
    float valueDiff = 0.05f*floor(idF/100.f);
    if(valueDiff>0.7f){
        LogError("Maximum number of picking colors reached at ID : " << id);
        return NULL;
    }
    vec3 hsv = vec3(idF * M_PI - floor(idF * M_PI), 0.5f, 0.95f-valueDiff);
    vec3 rgb = hsv2rgb(hsv);
    DataVec3UINT8::type rgbUINT8;
    DataVec3UINT8::get()->vec3ToValue(rgb*255.f, &rgbUINT8);

    return new PickingObject(id, rgbUINT8);
}

void PickingManager::performUniqueColorGenerationTest(int iterations){
    std::vector<DataVec3UINT8::type> colorVec;
    bool passed = true;    

    for(int i=0; i<iterations; i++){
        float idF = static_cast<float>(i);
        float valueDiff = 0.05f*floor(idF/100.f);
        if(valueDiff>0.85f){
            LogError("Maximum number of picking colors reached at ID : " << i);
            return;
        }
        vec3 hsv = vec3(idF * M_PI - floor(idF * M_PI), 0.5f, 0.95f-valueDiff);
        vec3 rgb = hsv2rgb(hsv);
        DataVec3UINT8::type rgbUINT8;
        DataVec3UINT8::get()->vec3ToValue(rgb*255.f, &rgbUINT8);

        if(std::find(colorVec.begin(), colorVec.end(), rgbUINT8)!=colorVec.end()){
            ivec3 ic = ivec3(rgbUINT8.x, rgbUINT8.y, rgbUINT8.z);
            LogInfo("Duplicate Picking Color : (" << ic.x << "," << ic.y << "," << ic.z << ") at iteration " << i << " with valueDiff " << valueDiff);
            passed = false;
            return;
        }
        else
            colorVec.push_back(rgbUINT8);
    }
    if(passed)
        LogInfo("performUniqueColorGenerationTest passed with " << iterations << " iterations");
}

} // namespace
