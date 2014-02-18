#ifndef IVW_GEOMETRYSOURCE_H
#define IVW_GEOMETRYSOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/ports/geometryport.h>
#include <modules/base/processors/datasource.h>

namespace inviwo {

class IVW_MODULE_BASE_API GeometrySource : public DataSource<Geometry, GeometryOutport> {
public:
    GeometrySource();
    virtual ~GeometrySource();

    InviwoProcessorInfo();
};

} // namespace

#endif // IVW_GEOMETRYSOURCE_H

