#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/canvas.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/image/imagegl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasGL : public Canvas {
public:
    CanvasGL(uvec2 dimensions);

    virtual void initialize();
    virtual void initializeGL();
    virtual void deinitialize();
    virtual void activate();
    void render(const Image*);
    virtual void repaint();
    virtual void resize(uvec2 size);
    virtual void update();

protected:
    void renderImage();
    void renderNoise();
    void renderImagePlaneQuad();

private:
    static bool glewInitialized_;
    const ImageGL* image_;
    Shader* shader_;
    Shader* noiseShader_;

};

} // namespace

#endif // IVW_CANVASGL_H
