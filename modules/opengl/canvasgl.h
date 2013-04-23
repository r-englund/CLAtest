#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/util/canvas.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasGL : public Canvas {
public:
    CanvasGL(uvec2 dimensions);

    virtual void initialize();
    virtual void initializeGL();
    virtual void deinitialize();
    virtual void activate();
    virtual void repaint();
    virtual void resize(uvec2 size);
    virtual void update();

private:
    static bool glewInitialized_;

};

} // namespace

#endif // IVW_CANVASGL_H
