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

/**
* Calculates minimum and maximum values in image.
* @param data is the data in which we want to find the values.
* @param dim is the dimensions of the data.
* @return the minimum and maximum values of data.
**/
uvec2 MinMaxProcessor::calculateMinMaxValues(const uint8_t *data, int size) const{
    uvec2 minmax = uvec2(data[0], data[0]);
    for(int i = 0; i < size; i++){
        if(data[i] < minmax.x)
            minmax.x = data[i];
        if(data[i] > minmax.y)
            minmax.y = data[i];
    }
    return minmax;
}

void MinMaxProcessor::process() {
    //Get images from in- and outport.
    const Image* inputImage = inport0_.getData();
    Image* outImage = outport_.getEditableData();
       
    //Get ram representation
    const ImageRAM *imageRam = inputImage->getRepresentation<ImageRAM>();

    const uint8_t *data = static_cast<const uint8_t*>(imageRam->getData());

    uvec2 dim = inputImage->size();
    int width = dim.x,
        height = dim.y;
    
    //Calculate minmax values
    uvec2 minmax = calculateMinMaxValues(data, width * height * 4);

    //Print results
    LogInfo(" Image minimum: " << minmax.x << " maximum: " << minmax.y);
    
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->resize(dim);

    activateTarget(outport_);

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    shader_->setUniform("dimension_", vec2(1.f/dim.x,  1.f/dim.y));
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();
}

} // namespace
