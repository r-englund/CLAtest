#include "imagesource.h"

namespace inviwo {

ImageSource::ImageSource()
    : ProcessorGL(),
    outport_(Port::OUTPORT, "image.outport"),
    imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/swirl.tga")
{
    addPort(outport_);
    addProperty(imageFileName_);
}

ImageSource::~ImageSource() {}

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

    Image* outImage = outport_.getData();
    ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    uvec2 csize = outImageGL->getDimension();

    Texture2D* testTex = new Texture2D(csize, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
    glActiveTexture(GL_TEXTURE0);
    testTex->loadTexture(imageFileName_.get());

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    shader_->setUniform("dimension_", vec2(1.f/csize[0],  1.f/csize[1]));
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();
}

} // namespace
