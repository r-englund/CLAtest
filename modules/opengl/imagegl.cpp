#include "imagegl.h"

namespace inviwo {

    ImageGL::ImageGL()
        : ImageRepresentation(),
        dimensions_(ivec2(256,256)) //TODO: use actual value
    {
        initialize();
    }

    ImageGL::ImageGL(ivec2 dimensions)
        : ImageRepresentation(),
        dimensions_(dimensions)
    {
        initialize();
    }

    ImageGL::~ImageGL() {}

    void ImageGL::initialize() {
        frameBufferObject_ = new FrameBufferObject();
        frameBufferObject_->activate();
        colorTexture_ = new Texture2D(dimensions_, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
        colorTexture_->bind();
        colorTexture_->upload();
        frameBufferObject_->attachTexture(colorTexture_);
        depthTexture_ = new Texture2D(dimensions_, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT, GL_LINEAR);
        depthTexture_->bind();
        depthTexture_->upload();
        frameBufferObject_->attachTexture(depthTexture_, GL_DEPTH_ATTACHMENT);
        frameBufferObject_->deactivate();
        frameBufferObject_->checkStatus();
    }

    void ImageGL::deinitialize() {
        delete frameBufferObject_;
        frameBufferObject_ = 0;
        delete colorTexture_;
        colorTexture_ = 0;
        delete depthTexture_;
        depthTexture_ = 0;
    }

    void ImageGL::activateBuffer() {
        frameBufferObject_->activate();
        glViewport(0, 0, colorTexture_->getWidth(), colorTexture_->getHeight());
    }

    void ImageGL::deactivateBuffer() {
        frameBufferObject_->deactivate();
    }

    void ImageGL::bindColorTexture(GLenum texUnit) {
        glActiveTexture(texUnit);
        colorTexture_->bind();

    }

    void ImageGL::bindDepthTexture(GLenum texUnit) {
        glActiveTexture(texUnit);
        depthTexture_->bind();
    }

    void ImageGL::bindTextures(GLenum colorTexUnit, GLenum depthTexUnit) {
        bindColorTexture(colorTexUnit);
        bindDepthTexture(depthTexUnit);
    }

} // namespace
