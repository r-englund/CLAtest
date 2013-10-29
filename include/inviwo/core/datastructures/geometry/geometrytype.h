#ifndef IVW_GEOMETRYTYPE_H
#define IVW_GEOMETRYTYPE_H

namespace inviwo {

enum RenderType{
    NOT_SPECIFIED,
    POINTS,
    LINES,
    TRIANGLES,
    NUMBER_OF_RENDER_TYPES
};

enum ConnectivityType{
    NONE,
    STRIP,
    LOOP,
    FAN,
    NUMBER_OF_CONNECTIVITY_TYPES
};

} // namespace

#endif // IVW_GEOMETRYTYPE_H
