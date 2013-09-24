#include "canvasgl.h"
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
const GeometryGL* CanvasGL::screenAlignedSquareGL_ = NULL;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions) {
    imageGL_ = NULL;
    shader_ = NULL;
    noiseShader_ = NULL;
}

CanvasGL::~CanvasGL() {}

void CanvasGL::initialize() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
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

void CanvasGL::initializeGL() {
    if (!glewInitialized_) {
        LogInfo("Initializing GLEW");
        glewInit();
        LGL_ERROR;
        glewInitialized_ = true;
    }
}

void CanvasGL::initializeSquare(){
    if (!screenAlignedSquareGL_) {
        screenAlignedSquareGL_ = screenAlignedSquare_->getRepresentation<GeometryGL>();
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
        //Pre-download incoming image (for Picking etc)
        imageGL_->getColorTexture()->downloadToPBO();
    } else {
        imageGL_ = NULL;
        renderNoise();
    }
}

void CanvasGL::repaint() {
    update();
}

void CanvasGL::resize(uvec2 size) {
    Canvas::resize(size);
    glViewport(0, 0, size[0], size[1]);
}

void CanvasGL::update() {
    Canvas::update();
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
    imageGL_->unbindDepthTexture();
}

void CanvasGL::renderNoise() {
    noiseShader_->activate();
    noiseShader_->setUniform("dimension_", vec2( 1.f / dimensions_[0],  1.f / dimensions_[1]) );
    renderImagePlaneRect();
    noiseShader_->deactivate();
}

void CanvasGL::renderTexture(GLint unitNumber) {
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    shader_->setUniform("dimension_", vec2( 1.f / dimensions_[0],  1.f / dimensions_[1]) );
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_.x, dimensions_.y);
    renderImagePlaneRect();
    shader_->deactivate();
}

} // namespace
