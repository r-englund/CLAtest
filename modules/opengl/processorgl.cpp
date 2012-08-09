#include "processorgl.h"

namespace inviwo {

    ProcessorGL::ProcessorGL()
        : Processor()
    {}
    ProcessorGL::~ProcessorGL() {}

    void ProcessorGL::activateTarget(ImagePort outport) {
        Image* outImage = outport.getData();
        ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
        outImageGL->activateBuffer();
    }

    void ProcessorGL::deactivateCurrentTarget() {
        FrameBufferObject::deactivate();
    }

    void ProcessorGL::bindColorTexture(ImagePort inport, GLenum texUnit) {
        Image* inImage = inport.getData();
        ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->bindColorTexture(texUnit);
    }

    void ProcessorGL::renderImagePlaneQuad() const {
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

} // namespace
