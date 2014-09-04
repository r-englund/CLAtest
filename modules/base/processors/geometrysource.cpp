#include "geometrysource.h"

namespace inviwo {

ProcessorClassIdentifier(GeometrySource, "org.inviwo.GeometrySource");
ProcessorDisplayName(GeometrySource,  "Geometry Source");
ProcessorTags(GeometrySource, Tags::None);
ProcessorCategory(GeometrySource, "Data Input");
ProcessorCodeState(GeometrySource, CODE_STATE_EXPERIMENTAL);

GeometrySource::GeometrySource() : DataSource<Geometry, GeometryOutport>() {
    DataSource<Geometry, GeometryOutport>::file_.setContentType("geometry");
    DataSource<Geometry, GeometryOutport>::file_.setDisplayName("Geometry file");
}

GeometrySource::~GeometrySource() {
}

} // namespace

