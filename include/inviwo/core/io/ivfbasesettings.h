#ifndef IVW_IVFBASESETTINGS_H
#define IVW_IVFBASESETTINGS_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"

namespace inviwo {

class IVW_CORE_API IvfBaseSettings : public IvwSerializable {
public:
    IvfBaseSettings();
    ~IvfBaseSettings() {}

    //get functions    
    vec3 getSliceThickness() const {return sliceThickness_;}
    float getTimeStep() const {return timeStep_;}
    std::string getUnit() const {return unit_;}
    std::string getModality() const {return modality_;}
    mat4 getTransformationMatrix() const {return transformationMatrix_;}

    //set functions
    void setSliceThickness(const vec3& thickness) {sliceThickness_ = thickness;}
    void setTimeStep(const float& timeStep) {timeStep_ = timeStep;}
    void setUnit(const std::string& unit) {unit_ = unit;}
    void setModality(const std::string& modality) {modality_ = modality;}
    void setTransformationMatrix(const mat4& tfMat) {transformationMatrix_ = tfMat;}


    //serialization
    virtual void serialize(IvwSerializer& s) const;

    //de-serialization
    virtual void deserialize(IvwDeserializer& d);
private:
    vec3 sliceThickness_;
    float timeStep_;
    std::string unit_;
    std::string modality_;
    mat4 transformationMatrix_;
};

} // namespace

#endif // IVW_IVFBASESETTINGS_H
