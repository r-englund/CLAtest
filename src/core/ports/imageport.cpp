#include "inviwo/core/ports/imageport.h"

namespace inviwo {

    ImagePort::ImagePort(PortDirection direction, std::string identifier)
        : DataPort<Image>(direction, identifier)
    {}

    ImagePort::~ImagePort() {}

    void ImagePort::initialize() {
        data_ = new Image(dimensions_);
    }

    void ImagePort::deinitialize() {
        delete data_;
    }

    void ImagePort::resize(ivec2 dimensions) {
        dimensions_ = dimensions;
        data_->resize(dimensions);
    }

} // namespace
