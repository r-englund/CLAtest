#include "processorgl.h"

namespace inviwo {

ProcessorGL::ProcessorGL()
    : Processor()
{}
ProcessorGL::~ProcessorGL() {}

void ProcessorGL::activateTarget(ImageOutport& outport){
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->activateBuffer();
}

void ProcessorGL::deactivateCurrentTarget() {
    FrameBufferObject::deactivate();
}

void ProcessorGL::clearCurrentTarget() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ProcessorGL::activateAndClearTarget(ImageOutport& outport){
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->activateBuffer();
    clearCurrentTarget();
}

void ProcessorGL::bindColorTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindColorTexture(texUnit);
}

void ProcessorGL::bindDepthTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindDepthTexture(texUnit);
}

void ProcessorGL::bindColorDepthTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindColorTexture(colorTexUnit);
    inImageGL->bindDepthTexture(depthTexUnit);
}

void ProcessorGL::unbindColorTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindColorTexture();
}

void ProcessorGL::unbindDepthTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindDepthTexture();
}

void ProcessorGL::unbindColorDepthTextures(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindColorTexture();
    inImageGL->unbindDepthTexture();
}

void ProcessorGL::setGlobalShaderParameters(Shader* shader) {
    vec2 screenDimensions = vec2(0.0f,0.0f);
    std::vector<Outport*> outports = getOutports();
    for (size_t i=0; i<outports.size(); i++) {
        ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(outports[i]);
        if (imageOutport) {
            screenDimensions = imageOutport->getDimensions();
            continue;
        }
    }
    shader->setUniform("screenDim_", screenDimensions);
    shader->setUniform("screenDimRCP_", vec2(1.0f,1.0f)/screenDimensions);
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
