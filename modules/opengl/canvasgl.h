#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/canvas.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasGL : public Canvas {
public:
    CanvasGL(uvec2 dimensions);
    virtual ~CanvasGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual void activate();
    void render(const Image* im, ImageLayerType layer = COLOR_LAYER);
    virtual void repaint();
    virtual void resize(uvec2 size);
    virtual void update();

    static inline void renderImagePlaneRect(){
        screenAlignedSquareGL_->render();
    }

protected:
    virtual void initializeGL();
    virtual void initializeSquare();

    void renderColor();
    void renderDepth();
    void renderPicking();
    void renderNoise();
    void renderTexture(GLint);

private:
    static bool glewInitialized_;
    static const GeometryGL* screenAlignedSquareGL_;

    const ImageGL* imageGL_;
    Shader* shader_;
    Shader* noiseShader_;

};

} // namespace

#endif // IVW_CANVASGL_H
