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