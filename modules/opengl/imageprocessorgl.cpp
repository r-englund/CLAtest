#include "imageprocessorgl.h"

namespace inviwo {

ImageProcessorGL::ImageProcessorGL()
    : ProcessorGL(),
    programFileName_("passthrough")
{}

ImageProcessorGL::ImageProcessorGL(std::string programFileName)
    : ProcessorGL(),
    programFileName_(programFileName)
{}

ImageProcessorGL::~ImageProcessorGL() {}

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
    // FIXME: implement this function
    return vec2(0.0f, 1.0f);
}

} // namespace
