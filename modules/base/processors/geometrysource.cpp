#include "geometrysource.h"

namespace inviwo {

ProcessorClassName(GeometrySource, "GeometrySource");
ProcessorCategory(GeometrySource, "Data Input");
ProcessorCodeState(GeometrySource, CODE_STATE_EXPERIMENTAL);

GeometrySource::GeometrySource() : DataSource<Geometry, GeometryOutport>() {
    DataSource<Geometry, GeometryOutport>::file_.setDisplayName("Geometry file");
}

GeometrySource::~GeometrySource() {
}

} // namespace

