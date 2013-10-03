#include <inviwo/core/util/formats.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>


namespace inviwo {

ImageGL::ImageGL(uvec2 dimensions, ImageType type, const DataFormatBase* format, Texture2D* colorTexture, Texture2D* depthTexture, Texture2D* pickingTexture)
: ImageRepresentation(dimensions, type, format), colorTexture_(colorTexture), depthTexture_(depthTexture), pickingTexture_(pickingTexture)
{
    initialize();
}

ImageGL::~ImageGL() {
    deinitialize();
}

void ImageGL::initialize() {
    frameBufferObject_ = new FrameBufferObject();
    frameBufferObject_->activate();
    colorConstTexture_ = colorTexture_;
    depthConstTexture_ = depthTexture_;
    pickingConstTexture_ = pickingTexture_;
    pickingAttachmentID_ = 0;
    if(typeContainsColor(getImageType())){
        if(!colorTexture_){
            createAndAddLayer(COLOR_LAYER);
        }
        else{
            colorTexture_->bind();
        }
        frameBufferObject_->attachTexture(colorTexture_);
    }
    if(typeContainsDepth(getImageType())){
        if(!depthTexture_){
            createAndAddLayer(DEPTH_LAYER);
        }
        else{
            depthTexture_->bind();
        }
        frameBufferObject_->attachTexture(depthTexture_, static_cast<GLenum>(GL_DEPTH_ATTACHMENT));
    }
    if(typeContainsPicking(getImageType())){
        if(!pickingTexture_){
            createAndAddLayer(PICKING_LAYER);
        }
        else{
            pickingTexture_->bind();
        }
        //Attach to last color attachment
        pickingAttachmentID_ = frameBufferObject_->attachTexture(pickingTexture_, 0, true); 
    }
    frameBufferObject_->deactivate();
    frameBufferObject_->checkStatus();
}

void ImageGL::deinitialize() {
    frameBufferObject_->deactivate();
    delete frameBufferObject_;
    frameBufferObject_ = NULL;
    if(colorTexture_){
        delete colorTexture_;
        colorTexture_ = NULL;
    }
    colorConstTexture_ = NULL;
    if(depthTexture_){
        delete depthTexture_;
        depthTexture_ = NULL;
    }
    depthConstTexture_ = NULL;
    if(pickingTexture_){
        delete pickingTexture_;
        pickingTexture_ = NULL;
    }
    pickingConstTexture_ = NULL;
}

DataRepresentation* ImageGL::clone() const {
    Texture2D* colorTexture = NULL;
    Texture2D* depthTexture = NULL;
    Texture2D* pickingTexture = NULL;
    if(colorConstTexture_)
        colorTexture = colorConstTexture_->clone();
    if(depthConstTexture_)
        depthTexture = depthConstTexture_->clone();
    if(pickingConstTexture_)
        pickingTexture = pickingConstTexture_->clone();
    ImageGL* newImageGL = new ImageGL(dimensions_, getImageType(), getDataFormat(), colorTexture, depthTexture, pickingTexture);
    return newImageGL;
}

void ImageGL::useInputSource(ImageLayerType layer, const Image* src){
    const ImageGL* srcGL = src->getRepresentation<ImageGL>();
    if(layer == COLOR_LAYER && !typeContainsColor(getImageType()) && !colorTexture_){
        colorConstTexture_ = srcGL->getColorTexture();
    }
    if(layer == DEPTH_LAYER &&!typeContainsDepth(getImageType()) && !depthTexture_){
        depthConstTexture_ = srcGL->getDepthTexture();
    }
    if(layer == PICKING_LAYER &&!typeContainsPicking(getImageType()) && !pickingTexture_){
        pickingConstTexture_ = srcGL->getPickingTexture();
    }
}

void ImageGL::createAndAddLayer(ImageLayerType layer){
    if(layer == COLOR_LAYER){
        colorTexture_ = new Texture2D(getDimensions(), getGLFormats()->getGLFormat(getDataFormatId()), GL_LINEAR);
        colorTexture_->upload(NULL);
        colorConstTexture_ = colorTexture_;
    }
    else if(layer == DEPTH_LAYER){
        depthTexture_ = new Texture2D(getDimensions(), GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_FLOAT, GL_LINEAR);
        depthTexture_->upload(NULL);
        depthConstTexture_ = depthTexture_;
    }
    else if(layer == PICKING_LAYER){
        pickingTexture_ = new Texture2D(getDimensions(), getGLFormats()->getGLFormat(getDataFormatId()), GL_LINEAR);
        pickingTexture_->upload(NULL);
        pickingConstTexture_ = pickingTexture_;
    }
}

void ImageGL::activateBuffer() {
    invalidatePBOs();
    frameBufferObject_->activate();
    glViewport(0, 0, colorConstTexture_->getWidth(), colorConstTexture_->getHeight());
}

void ImageGL::deactivateBuffer() {
    frameBufferObject_->deactivate();
}

void ImageGL::bindColorTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    colorConstTexture_->bind();
}

void ImageGL::bindDepthTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    depthConstTexture_->bind();
}

void ImageGL::bindPickingTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    pickingConstTexture_->bind();
}

void ImageGL::bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) const {
    bindColorTexture(colorTexUnit);
    bindDepthTexture(depthTexUnit);
}

void ImageGL::unbindDepthTexture() const {
    depthConstTexture_->unbind();
}

void ImageGL::unbindColorTexture() const {
    colorConstTexture_->unbind();
}

void ImageGL::unbindPickingTexture() const {
    pickingConstTexture_->unbind();
}

void ImageGL::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    if(colorTexture_){
        colorTexture_->unbind();
        colorTexture_->resize(dimensions_);    
        colorTexture_->unbind();
    }
    
    if(depthTexture_){
        depthTexture_->unbind();
        depthTexture_->resize(dimensions_);
        depthTexture_->unbind();
    }

    if(pickingTexture_){
        pickingTexture_->unbind();
        pickingTexture_->resize(dimensions_);
        pickingTexture_->unbind();
    }
}

void ImageGL::invalidatePBOs(){
    if(colorTexture_)
        colorTexture_->invalidatePBO();
    if(depthTexture_)
        depthTexture_->invalidatePBO();
    if(pickingTexture_)
        pickingTexture_->invalidatePBO();
}

bool ImageGL::copyAndResizeImage(DataRepresentation* targetRep) {
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);
    ImageGL* source = this;

    //Need to render to screen, as DEPTH attachment can't be linearly sampled using blitting.

    //Resize by FBO blit
    const FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    const Texture2D* sTex = source->getColorTexture();
    Texture2D* tTex = target->getColorTexture();

    srcFBO->setRead_Blit(); 
    tgtFBO->setDraw_Blit();

    glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
                         0, 0, tTex->getWidth(), tTex->getHeight(),
                         GL_COLOR_BUFFER_BIT, GL_LINEAR);

         
    srcFBO->setRead_Blit(false); 
    tgtFBO->setDraw_Blit(false);        
    FrameBufferObject::deactivate();

    LGL_ERROR;

    return true;
}

bool ImageGL::updateFrom(const ImageGL* source) {
    ImageGL* target = this;

    //Primarily Copy by FBO blitting, all from source FBO to target FBO
    const FrameBufferObject* srcFBO = source->getFBO();
    FrameBufferObject* tgtFBO = target->getFBO();
    const Texture2D* sTex = source->getColorTexture();
    Texture2D* tTex = target->getColorTexture();

    std::vector<GLenum> srcIDs = srcFBO->getColorAttachementIDs();
    std::vector<GLenum> targetIDs = tgtFBO->getColorAttachementIDs();

    size_t colorIDS = std::min(srcIDs.size(), targetIDs.size());

    srcFBO->setRead_Blit(); 
    tgtFBO->setDraw_Blit();

    GLbitfield mask = GL_COLOR_BUFFER_BIT;
    if(srcFBO->hasDepthAttachment() && tgtFBO->hasDepthAttachment())
        mask |= GL_DEPTH_BUFFER_BIT;
    if(srcFBO->hasStencilAttachment() && tgtFBO->hasStencilAttachment())
        mask |= GL_STENCIL_BUFFER_BIT;

    if(colorIDS > 0){
        glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
                             0, 0, tTex->getWidth(), tTex->getHeight(),
                             mask, GL_NEAREST);

        for(size_t i = 1; i < colorIDS; i++){
            glReadBuffer(srcIDs[i]);
            glDrawBuffer(targetIDs[i]);
            glBlitFramebufferEXT(0, 0, sTex->getWidth(), sTex->getHeight(),
                                 0, 0, tTex->getWidth(), tTex->getHeight(),
                                 GL_COLOR_BUFFER_BIT, GL_NEAREST);
        }
        glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
        glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
    }

    srcFBO->setRead_Blit(false); 
    tgtFBO->setDraw_Blit(false);        
    FrameBufferObject::deactivate();
    LGL_ERROR;

    //Secondary copy using PBO

    //Depth texture
    if((mask & GL_DEPTH_BUFFER_BIT) == 0){
        sTex = source->getDepthTexture();
        tTex = target->getDepthTexture();

        if(sTex && tTex)
            tTex->uploadFromPBO(sTex);
    }
    LGL_ERROR;

    //Picking texture
    if (pickingAttachmentID_ != 0){
        if(std::find(srcIDs.begin(), srcIDs.end(), pickingAttachmentID_) != srcIDs.end()){
            sTex = source->getPickingTexture();
            tTex = target->getPickingTexture();

            if(sTex && tTex)
                tTex->uploadFromPBO(sTex);
        }
    }
    LGL_ERROR;

    return true;
}

} // namespace
