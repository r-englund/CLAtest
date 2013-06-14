#ifndef IVW_GEOMETRYREPRESENTATION_H
#define IVW_GEOMETRYREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API GeometryRepresentation : public DataRepresentation {

public:
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

    GeometryRepresentation();
    virtual ~GeometryRepresentation();
    virtual void performOperation(DataOperation*) const;
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const;
    virtual void render(RenderType = GeometryRepresentation::NOT_SPECIFIED) const = 0;
};

} // namespace

#endif // IVW_GEOMETRYREPRESENTATION_H
