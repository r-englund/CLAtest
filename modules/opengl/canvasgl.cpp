#include "canvasgl.h"
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/geometry/attributebuffergl.h>
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
GLuint CanvasGL::screenAlignedVerticesId_ = 0;
GLuint CanvasGL::screenAlignedTexCoordsId_ = 1;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions) {
    imageGL_ = NULL;
    shader_ = NULL;
    noiseShader_ = NULL;
}

CanvasGL::~CanvasGL() {}

void CanvasGL::initialize() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    LGL_ERROR;
    shader_ = new Shader("img_texturequad.frag");
    LGL_ERROR;
    noiseShader_ = new Shader("img_noise.frag");
    LGL_ERROR;
}

void CanvasGL::initializeGLEW() {
    if (!glewInitialized_) {
        LogInfo("Initializing GLEW");
        glewInit();
        LGL_ERROR;
        glewInitialized_ = true;
    }
}

void CanvasGL::initializeSquare() {
    const Mesh* screenAlignedRectMesh = dynamic_cast<const Mesh*>(screenAlignedRect_);
    if (screenAlignedRectMesh) {
        screenAlignedVerticesId_ = screenAlignedRectMesh->getAttributes(0)->getRepresentation<BufferGL>()->getId();
        screenAlignedTexCoordsId_ = screenAlignedRectMesh->getAttributes(1)->getRepresentation<BufferGL>()->getId();
        LGL_ERROR;
    }
}

void CanvasGL::deinitialize() {
    delete shader_;
    shader_ = NULL;
    delete noiseShader_;
    noiseShader_ = NULL;
}

void CanvasGL::activate() {}

void CanvasGL::render(const Image* image, ImageLayerType layer){
    if (image) {
        imageGL_ = image->getRepresentation<ImageGL>();
        pickingContainer_->setPickingSource(image);
        switch(layer){
            case COLOR_LAYER:
                renderColor();
                break;
            case DEPTH_LAYER:
                renderDepth();
                break;
            case PICKING_LAYER:
                renderPicking();
                break;
            default:
                renderNoise();
        }
    } else {
        imageGL_ = NULL;
        renderNoise();
    }
}

void CanvasGL::resize(uvec2 size) {
    Canvas::resize(size);
    glViewport(0, 0, size[0], size[1]);
}

void CanvasGL::glSwapBuffers(){
}

void CanvasGL::update() {
    if (imageGL_) {
        renderColor();
    } else {
        renderNoise();
    }
}

void CanvasGL::renderColor() {
    TextureUnit textureUnit;
    imageGL_->bindColorTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    imageGL_->unbindColorTexture();
}

void CanvasGL::renderDepth() {
    TextureUnit textureUnit;
    imageGL_->bindDepthTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    imageGL_->unbindDepthTexture();
}

void CanvasGL::renderPicking() {
    TextureUnit textureUnit;
    imageGL_->bindPickingTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    imageGL_->unbindPickingTexture();
}

void CanvasGL::renderNoise() {
    activate();
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_[0], dimensions_[1]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    noiseShader_->activate();
    renderImagePlaneRect();
    noiseShader_->deactivate();
    glSwapBuffers();
}

void CanvasGL::renderTexture(GLint unitNumber) {
    activate();
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_[0], dimensions_[1]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    renderImagePlaneRect();
    shader_->deactivate();
    glDisable(GL_BLEND);
    glSwapBuffers();
}

} // namespace
