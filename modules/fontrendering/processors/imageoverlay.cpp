#include "imageoverlay.h"

namespace inviwo {

ProcessorClassName(ImageOverlay, "ImageOverlay"); 
ProcessorCategory(ImageOverlay, "Compositor");
ProcessorCodeState(ImageOverlay, CODE_STATE_EXPERIMENTAL);

ImageOverlay::ImageOverlay() {}
ImageOverlay::~ImageOverlay() {}
void ImageOverlay::initialize() {}
void ImageOverlay::deinitialize() {}
void ImageOverlay::process() {}

} // namespace