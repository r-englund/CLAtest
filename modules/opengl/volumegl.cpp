#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL()
    : volumeTexture_(0), VolumeRepresentation(ivec3(128,128,128), "UINT8") //TODO: use actual value
{
    initialize();
}

VolumeGL::VolumeGL(ivec3 dimensions)
    : volumeTexture_(0), VolumeRepresentation(dimensions, "UINT8")
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
