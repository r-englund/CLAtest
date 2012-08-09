#include "canvasprocessorgl.h"

namespace inviwo {

    CanvasProcessorGL::CanvasProcessorGL()
        : CanvasProcessor()
    {}

    void CanvasProcessorGL::initialize() {
        CanvasProcessor::initialize();
        shader_ = new Shader("img_texturequad.frag");
    }
    
    void CanvasProcessorGL::deinitialize() {
        delete shader_;
        CanvasProcessor::deinitialize();
    }

    void CanvasProcessorGL::renderImagePlaneQuad() const {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glDepthFunc(GL_ALWAYS);
        glBegin(GL_QUADS);
        glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
        glTexCoord2f(1.f, 0.f); glVertex2f( 1.f, -1.f);
        glTexCoord2f(1.f, 1.f); glVertex2f( 1.f,  1.f);
        glTexCoord2f(0.f, 1.f); glVertex2f(-1.f,  1.f);
        glEnd();
        glDepthFunc(GL_LESS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    void CanvasProcessorGL::process() {
        std::cout << "CanvasProcessor::process()" << std::endl;

        Image* inImage = inport_.getData();
        ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->bindColorTexture(GL_TEXTURE0);

        shader_->activate();
        shader_->setUniform("colorTex_", 0);
        renderImagePlaneQuad();
        shader_->deactivate();
    }

} // namespace
