#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include "inviwoopengl.h"
#include "inviwo/core/util/canvas.h"

namespace inviwo {

class CanvasGL : public Canvas {
public:
    CanvasGL(ivec2 dimensions);

    virtual void initialize();
    virtual void deinitialize();

};

} // namespace

#endif // IVW_CANVASGL_H
