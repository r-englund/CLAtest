#include "imagemixer.h"

#include <inviwo/core/datastructures/image/imageram.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

ProcessorClassName(ImageMixer, "ImageMixer"); 
ProcessorCategory(ImageMixer, "Image Operation");
ProcessorCodeState(ImageMixer, CODE_STATE_EXPERIMENTAL); 

ImageMixer::ImageMixer()
    : ProcessorGL(),
      inport0_("inport0"),
      inport1_("inport1"),
      outport_("outport", &inport0_, COLOR_ONLY),
      alpha_("alpha", "Alpha", 0.5f, 0.0f, 1.0f)
{
    addPort(inport0_);
    addPort(inport1_);
    addPort(outport_);
    addProperty(alpha_);
}

ImageMixer::~ImageMixer() {}

void ImageMixer::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_mix.frag");
}

void ImageMixer::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void ImageMixer::process() {    
    ivwAssert(inport0_.getData()!=0, "Inport0 empty.");
    ivwAssert(inport1_.getData()!=0, "Inport1 empty.");
    TextureUnit inportTexture0;
    TextureUnit inportTexture1;
    uvec2 csize = outport_.getData()->getDimension();    
    bindColorTexture(inport0_, inportTexture0.getEnum());
    bindColorTexture(inport1_, inportTexture1.getEnum());    
    activateTarget(outport_);
    shader_->activate();
    shader_->setUniform("inport0_", inportTexture0.getUnitNumber());
    shader_->setUniform("inport1_", inportTexture1.getUnitNumber());
    shader_->setUniform("alpha_", alpha_.get());
    shader_->setUniform("dimension_", vec2(1.f / csize[0], 1.f / csize[1]) );
    renderImagePlaneSquare();
    shader_->deactivate();
    deactivateCurrentTarget();
    unbindColorTexture(inport0_);
    unbindColorTexture(inport1_);
}

} // namespace
