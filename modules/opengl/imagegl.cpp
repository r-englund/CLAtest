#include <inviwo/core/util/formats.h>
#include "imagegl.h"
#include "glwrap/shader.h"
#include "glwrap/textureunit.h"


namespace inviwo {

ImageGL::ImageGL()
    : ImageRepresentation(uvec2(256,256), DataVec4UINT8())
{
    colorTexture_ = new Texture2D(dimensions_, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
    colorTexture_->upload(NULL);
    depthTexture_ = new Texture2D(dimensions_, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT, GL_LINEAR);
    depthTexture_->upload(NULL);
    initialize();
}

ImageGL::ImageGL(uvec2 dimensions)
    : ImageRepresentation(dimensions, DataVec4UINT8())
{
    colorTexture_ = new Texture2D(dimensions_, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
    colorTexture_->upload(NULL);
    depthTexture_ = new Texture2D(dimensions_, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT, GL_LINEAR);
    depthTexture_->upload(NULL);
    initialize();
}

ImageGL::ImageGL(Texture2D* colorTexture, uvec2 dimensions)
    : ImageRepresentation(dimensions, DataVec4UINT8())
{
    colorTexture_ = colorTexture;
    depthTexture_ = new Texture2D(dimensions_, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT, GL_LINEAR);
    depthTexture_->upload(NULL);
    initialize();
}

ImageGL::ImageGL(Texture2D* colorTexture, Texture2D* depthTexture, uvec2 dimensions)
: ImageRepresentation(dimensions, DataVec4UINT8())
{
    colorTexture_ = colorTexture;
    depthTexture_ = depthTexture;
    initialize();
}

ImageGL::~ImageGL() {}

void ImageGL::initialize() {
    frameBufferObject_ = new FrameBufferObject();
    frameBufferObject_->activate();
    colorTexture_->bind();
    frameBufferObject_->attachTexture(colorTexture_);
    depthTexture_->bind();
    frameBufferObject_->attachTexture(depthTexture_, GL_DEPTH_ATTACHMENT);
    frameBufferObject_->deactivate();
    frameBufferObject_->checkStatus();
}

void ImageGL::deinitialize() {
    frameBufferObject_->deactivate();
    delete frameBufferObject_;
    frameBufferObject_ = 0;
    colorTexture_->unbind();
    delete colorTexture_;
    colorTexture_ = 0;
    depthTexture_->unbind();
    delete depthTexture_;
    depthTexture_ = 0;
}

DataRepresentation* ImageGL::clone() const {
    Texture2D* colorTexture = colorTexture_->clone();
    Texture2D* depthTexture = depthTexture_->clone();
    ImageGL* newImageGL = new ImageGL(colorTexture, depthTexture, dimensions_);
    return newImageGL;
}

void ImageGL::activateBuffer() {
    frameBufferObject_->activate();
    glViewport(0, 0, colorTexture_->getWidth(), colorTexture_->getHeight());
}

void ImageGL::deactivateBuffer() {
    frameBufferObject_->deactivate();
}

void ImageGL::bindColorTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    colorTexture_->bind();
}

void ImageGL::bindDepthTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    depthTexture_->bind();
}

void ImageGL::bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) const {
    bindColorTexture(colorTexUnit);
    bindDepthTexture(depthTexUnit);
}

void ImageGL::unbindDepthTexture() const {
    depthTexture_->unbind();
}

void ImageGL::unbindColorTexture() const {
    colorTexture_->unbind();
}

void ImageGL::resize(uvec2 dimensions) {
    dimensions_ = dimensions;        
    colorTexture_->unbind();
    colorTexture_->resize(dimensions_);    
    colorTexture_->upload(NULL);
    colorTexture_->unbind();

    depthTexture_->unbind();
    depthTexture_->resize(dimensions_);
    depthTexture_->upload(NULL);
    depthTexture_->unbind();    
}

void ImageGL::copyAndResizeImage(DataRepresentation* targetRep) {
    
    ImageGL* source = this;
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);

    if (!target) return;

    //Resize by FBO blit
    FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    Texture2D* sTex = source->getColorTexture();
    Texture2D* tTex = target->getColorTexture();
    
    //FrameBufferObject::deactivate();
    //source->activateBuffer();
    srcFBO->setRead_Blit(); 
    //glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

    //FrameBufferObject::deactivate();
    //target->activateBuffer();
    tgtFBO->setDraw_Blit();
    //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);        
            
    //srcFBO->attachTexture(sTex);
    //tgtFBO->attachTexture(tTex);        
    //FrameBufferObject::deactivate();
    //glDisable(GL_SCISSOR_TEST);
    glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
                         0, 0, tTex->getWidth(), tTex->getHeight(),
                         GL_COLOR_BUFFER_BIT, GL_LINEAR);
         
    srcFBO->setRead_Blit(false); 
    tgtFBO->setDraw_Blit(false);        
    FrameBufferObject::deactivate();
}

} // namespace
