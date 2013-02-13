#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL()
    : VolumeRepresentation(ivec3(128,128,128), "UINT8"), volumeTexture_(0) //TODO: use actual value
{
    initialize();
}

VolumeGL::VolumeGL(ivec3 dimensions)
    : VolumeRepresentation(dimensions, "UINT8"), volumeTexture_(0)
{
    initialize();
}

VolumeGL::~VolumeGL() {}

void VolumeGL::initialize() {}

void VolumeGL::deinitialize() {}

void VolumeGL::bindTexture(GLenum texUnit) {
    glActiveTexture(texUnit);
    volumeTexture_->bind();
}

void VolumeGL::unbindTexture() {
    volumeTexture_->unbind();
}

} // namespace
