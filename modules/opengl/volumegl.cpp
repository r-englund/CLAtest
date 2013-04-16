#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL(uvec3 dimensions, DataFormatBase format)
    : VolumeRepresentation(dimensions, format), volumeTexture_(0)
{
    initialize();
}

VolumeGL::~VolumeGL() {}

void VolumeGL::initialize() {}

void VolumeGL::deinitialize() {}

void VolumeGL::bindTexture(GLenum texUnit) const{
    glActiveTexture(texUnit);
    volumeTexture_->bind();
}

void VolumeGL::unbindTexture() const{
    volumeTexture_->unbind();
}

} // namespace
