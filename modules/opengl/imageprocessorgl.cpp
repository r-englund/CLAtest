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
    ProcessorGL::deinitialize();
}

} // namespace
