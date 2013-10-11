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

void ProcessorGL::activateAndClearTarget(ImageOutport& outport) {
    activateTarget(outport);
    clearCurrentTarget();
}

void ProcessorGL::activateAndClearTargets(PortGroup& portGroup) {
    portGroup.activate();
    clearCurrentTarget();
}

void ProcessorGL::updateAndActivateTarget(ImageOutport& outport, ImageInport& inport){
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->updateFrom(inport.getData()->getRepresentation<ImageGL>());
    outImageGL->activateBuffer();
}

void ProcessorGL::bindColorTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindColorTexture(texUnit);
}

void ProcessorGL::bindColorTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->bindColorTexture(texUnit);
}

void ProcessorGL::bindDepthTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindDepthTexture(texUnit);
}

void ProcessorGL::bindDepthTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->bindDepthTexture(texUnit);
}

void ProcessorGL::bindPickingTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindPickingTexture(texUnit);
}

void ProcessorGL::bindPickingTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->bindPickingTexture(texUnit);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindColorTexture(colorTexUnit);
    inImageGL->bindDepthTexture(depthTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->bindColorTexture(colorTexUnit);
    outImageGL->bindDepthTexture(depthTexUnit);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit){
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->bindColorTexture(colorTexUnit);
    inImageGL->bindDepthTexture(depthTexUnit);
    inImageGL->bindPickingTexture(pickingTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit){
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->bindColorTexture(colorTexUnit);
    outImageGL->bindDepthTexture(depthTexUnit);
    outImageGL->bindPickingTexture(pickingTexUnit);
}

void ProcessorGL::unbindColorTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindColorTexture();
}

void ProcessorGL::unbindColorTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->unbindColorTexture();
}

void ProcessorGL::unbindDepthTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindDepthTexture();
}

void ProcessorGL::unbindDepthTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->unbindDepthTexture();
}

void ProcessorGL::unbindPickingTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindPickingTexture();
}

void ProcessorGL::unbindPickingTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->unbindPickingTexture();
}

void ProcessorGL::unbindTextures(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->unbindColorTexture();
    inImageGL->unbindDepthTexture();
    inImageGL->unbindPickingTexture();
}

void ProcessorGL::unbindTextures(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->unbindColorTexture();
    outImageGL->unbindDepthTexture();
    outImageGL->unbindPickingTexture();
}

void ProcessorGL::setTextureParameters(const ImageInport& inport, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(inport.getDimensions());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f)/dimensions);
}

void ProcessorGL::setTextureParameters(const ImageOutport& outport, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(outport.getDimensions());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f)/dimensions);
}

void ProcessorGL::setGlobalShaderParameters(Shader* shader) {
    vec2 screenDimensions = vec2(0.0f,0.0f);
    std::vector<Outport*> outports = getOutports();
    for (size_t i=0; i<outports.size(); i++) {
        ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(outports[i]);
        if (imageOutport) {
            screenDimensions = imageOutport->getDimensions();
            break;
        }
    }
    shader->setUniform("screenDim_", screenDimensions);
    shader->setUniform("screenDimRCP_", vec2(1.0f,1.0f)/screenDimensions);
}

} // namespace
