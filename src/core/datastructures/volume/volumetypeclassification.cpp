/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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
