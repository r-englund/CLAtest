#include "minmaxprocessor.h"

#include <cmath>

namespace inviwo {

MinMaxProcessor::MinMaxProcessor()
    : ProcessorGL(),      
	inport0_(Port::INPORT, "inport0"),
	outport_(Port::OUTPORT, "outport"){
    addPort(inport0_);
    addPort(outport_);
}

MinMaxProcessor::~MinMaxProcessor() {}

Processor* MinMaxProcessor::create() const {
    return new MinMaxProcessor();
}

void MinMaxProcessor::initialize() {
    Processor::initialize();
    shader_ = new Shader("img_texturequad.frag");
}

void MinMaxProcessor::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}


void MinMaxProcessor::process() {
    Image* outImage = outport_.getData();
    Image* inputImage = inport0_.getData();
    ImageRAM *imageRam = inputImage->getRepresentation<ImageRAM>();
    
    GLubyte *data = static_cast<GLubyte*>(imageRam->getData());
    
    outImage->addRepresentation(imageRam);
    
    uvec2 dim = inputImage->size();
    int width = dim.x,
        height = dim.y;
    activateTarget(outport_);

    Texture2D* texture = new Texture2D(uvec2(width, height), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_NEAREST);


	texture->setTexels(data);
	texture->setWidth(width);
	texture->setHeight(height);

	texture->upload();

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    shader_->setUniform("dimension_", vec2(1.f/width,  1.f/height));
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();

}

} // namespace
