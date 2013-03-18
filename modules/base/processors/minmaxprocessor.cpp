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
     Image* outImage = outport_.getData();
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
uvec2 MinMaxProcessor::calculateMinMaxValues(uint8_t *data, int size) const{
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
    Image* inputImage = inport0_.getData();
    Image* outImage = outport_.getData();
    
    //Add a GL representation to outport if there isn't any already. This should be done in data.h,
    //when it is implemented, the if wont be nessecary.
    if(outImage->getRepresentation<ImageGL>() == NULL) //This wont be 
        outImage->addRepresentation(new ImageGL());
    
    //Get ram representation
    ImageRAM *imageRam = inputImage->getRepresentation<ImageRAM>();

    uint8_t *data = static_cast<uint8_t*>(imageRam->getData());

    uvec2 dim = inputImage->size();
    int width = dim.x,
        height = dim.y;
    
    //Calculate minmax values
    uvec2 minmax = calculateMinMaxValues(data, width * height * 4);

    //Print results
    LogInfoS(getClassName(), " Image minimum: " << minmax.x << " maximum: " << minmax.y);

    //Upload to GL (Because RAM2GL isn't working)
    Texture2D* texture = new Texture2D(uvec2(width, height), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_NEAREST);

	texture->setTexels(data);
	texture->setWidth(width);
	texture->setHeight(height);

    activateTarget(outport_);
	
    texture->upload();

    uvec2 csize = outImage->size();

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    shader_->setUniform("dimension_", vec2(1.f/csize.x,  1.f/csize.y));
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();
    delete texture;
}

} // namespace
