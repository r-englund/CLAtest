#include "inviwo/core/datastructures/volumetypeclassification.h"

namespace inviwo { 

UniformRectiLinearVolume::UniformRectiLinearVolume() : UniformData<Data::TYPE3D, RectiLinearVolume>() {
}

UniformRectiLinearVolume::~UniformRectiLinearVolume() {}

Data* UniformRectiLinearVolume::clone() {
    return new UniformRectiLinearVolume();
}

/*----------------------------------------------------------------------*/

UniformCurviLinearVolume::UniformCurviLinearVolume() : UniformData<Data::TYPE3D, CurviLinearVolume>() {
}

UniformCurviLinearVolume::~UniformCurviLinearVolume() {}

Data* UniformCurviLinearVolume::clone() {
    return new UniformCurviLinearVolume();
}

/*----------------------------------------------------------------------*/

NonUniformRectiLinearVolume::NonUniformRectiLinearVolume() : NonUniformData<Data::TYPE3D, RectiLinearVolume>() {
}

NonUniformRectiLinearVolume::~NonUniformRectiLinearVolume() {}

Data* NonUniformRectiLinearVolume::clone() {
    return new NonUniformRectiLinearVolume();
}

/*----------------------------------------------------------------------*/

NonUniformCurviLinearVolume::NonUniformCurviLinearVolume() : NonUniformData<Data::TYPE3D, CurviLinearVolume>() {
}

NonUniformCurviLinearVolume::~NonUniformCurviLinearVolume() {}

Data* NonUniformCurviLinearVolume::clone() {
    return new NonUniformCurviLinearVolume();
}

/*----------------------------------------------------------------------*/


} // namespace
