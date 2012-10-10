#include "imagesource.h"

namespace inviwo {

ImageSource::ImageSource()
    : ProcessorGL(),
    outport_(Port::OUTPORT, "image.outport"),
    imageFileName_("imageFileName", "Image file name", "D:/inviwo/data/images/swirl.tga")
{
    addPort(outport_);
    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {
   Processor::~Processor();
}

Processor* ImageSource::create() const {
    return new ImageSource();
}

void ImageSource::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_texturequad.frag");
}

void ImageSource::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void ImageSource::process() {
    activateTarget(outport_);

    Texture2D* testTex = new Texture2D(ivec2(256,256), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    testTex->loadTexture(imageFileName_.get());

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();
}

} // namespace
