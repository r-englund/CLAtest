#include "inviwo/core/io/ivfbasesettings.h"

namespace inviwo {

IvfBaseSettings::IvfBaseSettings() 
    : IvwSerializable()    
{}

void IvfBaseSettings::serialize(IvwSerializer& s) const {
    s.serialize("SliceThickness", sliceThickness_);
    s.serialize("TimeStep", timeStep_);
    s.serialize("Unit", unit_);
    s.serialize("Modality", modality_);
    s.serialize("TransformationMatrix", transformationMatrix_);
}

void IvfBaseSettings::deserialize(IvwDeserializer& d) {
    d.deserialize("SliceThickness", sliceThickness_);
    d.deserialize("TimeStep", timeStep_);
    d.deserialize("Unit", unit_);
    d.deserialize("Modality", modality_);
    d.deserialize("TransformationMatrix", transformationMatrix_);
}

} // namespace
