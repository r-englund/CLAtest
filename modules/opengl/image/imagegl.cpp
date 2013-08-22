#include <inviwo/core/util/formats.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>


namespace inviwo {

ImageGL::ImageGL(uvec2 dimensions, ImageType type, DataFormatBase format, Texture2D* colorTexture, Texture2D* depthTexture)
: ImageRepresentation(dimensions, type, format), colorTexture_(colorTexture), depthTexture_(depthTexture)
{
    initialize();
}

ImageGL::~ImageGL() {}

void ImageGL::initialize() {
    frameBufferObject_ = new FrameBufferObject();
    frameBufferObject_->activate();
    if(typeContainsColor(getImageType())){
        if(!colorTexture_){
            colorTexture_ = new Texture2D(getDimensions(), getGLFormats()->getGLFormat(getDataFormatId()), GL_LINEAR);
            colorTexture_->upload(NULL);
        }
        else
            colorTexture_->bind();
        colorConstTexture_ = colorTexture_;
        frameBufferObject_->attachTexture(colorTexture_);
    }
    if(typeContainsDepth(getImageType())){
        if(!depthTexture_){
            depthTexture_ = new Texture2D(getDimensions(), GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_FLOAT, GL_LINEAR);
            depthTexture_->upload(NULL);
        }
        else
            depthTexture_->bind();
        depthConstTexture_ = depthTexture_;
        frameBufferObject_->attachTexture(depthTexture_, GL_DEPTH_ATTACHMENT); 
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
}

DataRepresentation* ImageGL::clone() const {
    Texture2D* colorTexture = colorConstTexture_->clone();
    Texture2D* depthTexture = depthConstTexture_->clone();
    ImageGL* newImageGL = new ImageGL(dimensions_, getImageType(), getDataFormat(), colorTexture, depthTexture);
    return newImageGL;
}

void ImageGL::useInputSource(ImageLayerType layer, const Image* src){
    const ImageGL* srcGL = src->getRepresentation<ImageGL>();
    if(!typeContainsColor(getImageType()) && !colorTexture_){
        colorConstTexture_ = srcGL->getColorTexture();
    }
    if(!typeContainsDepth(getImageType()) && !depthTexture_){
        depthConstTexture_ = srcGL->getDepthTexture();
    }
}

void ImageGL::activateBuffer() {
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

void ImageGL::resize(uvec2 dimensions) {
    dimensions_ = dimensions;
    if(colorTexture_){
        colorTexture_->unbind();
        colorTexture_->resize(dimensions_);    
        colorTexture_->upload(NULL);
        colorTexture_->unbind();
    }
    
    if(depthTexture_){
        depthTexture_->unbind();
        depthTexture_->resize(dimensions_);
        depthTexture_->upload(NULL);
        depthTexture_->unbind();
    }
}

bool ImageGL::copyAndResizeImage(DataRepresentation* targetRep) {
    
    ImageGL* source = this;
    ImageGL* target = dynamic_cast<ImageGL*>(targetRep);

    ivwAssert(target!=0, "Target representation missing.");

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

    return true;
}

} // namespace
