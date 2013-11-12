#include "imageprocessorgl.h"

namespace inviwo {

ImageProcessorGL::ImageProcessorGL()
    : ProcessorGL(),
    programFileName_("pipethrough.frag")
{}

ImageProcessorGL::ImageProcessorGL(std::string programFileName)
    : ProcessorGL(),
    programFileName_(programFileName)
{}

void ImageProcessorGL::initialize() {
    ProcessorGL::initialize();
    program_ = new Shader(programFileName_);
}

void ImageProcessorGL::deinitialize() {
    delete program_;
    program_ = 0;
    ProcessorGL::deinitialize();
}

void ImageProcessorGL::initializeResources() {
    program_->rebuild();
}

vec2 ImageProcessorGL::computeDepthRange(ImageInport* inport) {
	vec2 depthRange = vec2(1.0f, 0.0f);
	const ImageRAM* imageRAM = (inport->getData())->getRepresentation<ImageRAM>();
	for (unsigned int x=0; x<imageRAM->getDimensions().x; x++) {
		for (unsigned int y=0; y<imageRAM->getDimensions().y; y++) {
			float curDepth = imageRAM->getDepthValue(uvec2(x,y));
			if (curDepth < depthRange.x) depthRange.x = curDepth;
			if (curDepth > depthRange.y) depthRange.y = curDepth;
		}
	}
	return depthRange;
}

} // namespace
