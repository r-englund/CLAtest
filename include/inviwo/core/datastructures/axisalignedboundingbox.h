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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_AXIS_ALIGNED_BOUNDING_BOX_H
#define IVW_AXIS_ALIGNED_BOUNDING_BOX_H

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

typedef struct
{
    vec3 pMin; // minium
    vec3 pMax; // maximum
} AxisAlignedBoundingBox;

} // namespace inviwo

#endif // IVW_AXIS_ALIGNED_BOUNDING_BOX_H