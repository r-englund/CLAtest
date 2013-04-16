#include "processorgl.h"

namespace inviwo {

    ProcessorGL::ProcessorGL()
        : Processor()
    {}
    ProcessorGL::~ProcessorGL() {}

    Processor* ProcessorGL::create() const {
        return new ProcessorGL();
    }

    void ProcessorGL::activateTarget(ImagePort outport) {
        Image* outImage = outport.getEditableData();
        ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
        outImageGL->activateBuffer();
    }

    void ProcessorGL::deactivateCurrentTarget() {
        FrameBufferObject::deactivate();
    }

    void ProcessorGL::bindColorTexture(ImagePort inport, GLenum texUnit) {
        const Image* inImage = inport.getData();
        const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->bindColorTexture(texUnit);
    }

    void ProcessorGL::bindDepthTexture(ImagePort inport, GLenum texUnit) {
        const Image* inImage = inport.getData();
        const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->bindDepthTexture(texUnit);
    }

    void ProcessorGL::unbindColorTexture(ImagePort inport) {
        const Image* inImage = inport.getData();
        const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->unbindColorTexture();
    }

    void ProcessorGL::unbindDepthTexture(ImagePort inport) {
        const Image* inImage = inport.getData();
        const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
        inImageGL->unbindDepthTexture();
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

    void ProcessorGL::serialize(IvwSerializer& s) const {
         Processor::serialize(s);
    }

    void ProcessorGL::deserialize(IvwDeserializer& d) {
         Processor::deserialize(d);
    }

} // namespace
