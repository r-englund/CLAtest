#include "canvasgl.h"
#include <modules/opengl/glwrap/textureunit.h>
namespace inviwo {

bool CanvasGL::glewInitialized_ = false;

CanvasGL::CanvasGL(uvec2 dimensions)
    : Canvas(dimensions) {
    image_ = NULL;
    shader_ = NULL;
    noiseShader_ = NULL;
}

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
        //LogInfo("Initializing GLEW");
        glewInit();
        LGL_ERROR;
        glewInitialized_ = true;
    }
}

void CanvasGL::deinitialize() {
    delete shader_;
    delete noiseShader_;
}

void CanvasGL::activate() {}

void CanvasGL::render(const Image* image, ImageLayerType layer){
    if (image) {
        image_ = image->getRepresentation<ImageGL>();
        switch(layer){
            case COLOR_LAYER:
                renderColor();
                break;
            case DEPTH_LAYER:
                renderDepth();
                break;
            default:
                renderNoise();
        }
    } else {
        image_ = NULL;
        renderNoise();
    }
}

void CanvasGL::repaint() {}

void CanvasGL::resize(uvec2 size) {
    Canvas::resize(size);
    glViewport(0, 0, size[0], size[1]);
}

void CanvasGL::update() {
    Canvas::update();
    if (image_) {
        renderColor();
    } else {
        renderNoise();
    }
}

void CanvasGL::renderColor() {
    TextureUnit textureUnit;
    image_->bindColorTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    image_->unbindColorTexture();
}

void CanvasGL::renderDepth() {
    TextureUnit textureUnit;
    image_->bindDepthTexture(textureUnit.getEnum());
    renderTexture(textureUnit.getUnitNumber());
    image_->unbindDepthTexture();
}

void CanvasGL::renderNoise() {
    noiseShader_->activate();
    noiseShader_->setUniform("dimension_", vec2( 1.f / dimensions_[0],  1.f / dimensions_[1]) );
    renderImagePlaneQuad();
    noiseShader_->deactivate();
}

void CanvasGL::renderTexture(GLint unitNumber) {
    shader_->activate();
    shader_->setUniform("tex_", unitNumber);
    shader_->setUniform("dimension_", vec2( 1.f / dimensions_[0],  1.f / dimensions_[1]) );
    //FIXME: glViewport should not be here, which indicates this context is not active.
    glViewport(0, 0, dimensions_.x, dimensions_.y);
    renderImagePlaneQuad();
    shader_->deactivate();
}

void CanvasGL::renderImagePlaneQuad() {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glDepthFunc(GL_ALWAYS);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f,  1.0f);
    glEnd();
    glDepthFunc(GL_LESS);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

} // namespace
