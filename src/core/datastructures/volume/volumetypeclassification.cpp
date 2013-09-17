#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

namespace inviwo { 

UniformRectiLinearVolume::UniformRectiLinearVolume() : UniformData<3, RectiLinearVolume>() {
}

UniformRectiLinearVolume::~UniformRectiLinearVolume() {}

Data* UniformRectiLinearVolume::clone() {
    return new UniformRectiLinearVolume();
}

/*----------------------------------------------------------------------*/

UniformCurviLinearVolume::UniformCurviLinearVolume() : UniformData<3, CurviLinearVolume>() {
}

UniformCurviLinearVolume::~UniformCurviLinearVolume() {}

Data* UniformCurviLinearVolume::clone() {
    return new UniformCurviLinearVolume();
}

/*----------------------------------------------------------------------*/

NonUniformRectiLinearVolume::NonUniformRectiLinearVolume() : NonUniformData<3, RectiLinearVolume>() {
}

NonUniformRectiLinearVolume::~NonUniformRectiLinearVolume() {}

Data* NonUniformRectiLinearVolume::clone() {
    return new NonUniformRectiLinearVolume();
}

/*----------------------------------------------------------------------*/

NonUniformCurviLinearVolume::NonUniformCurviLinearVolume() : NonUniformData<3, CurviLinearVolume>() {
}

NonUniformCurviLinearVolume::~NonUniformCurviLinearVolume() {}

Data* NonUniformCurviLinearVolume::clone() {
    return new NonUniformCurviLinearVolume();
}

/*----------------------------------------------------------------------*/


} // namespace
