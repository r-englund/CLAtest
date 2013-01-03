#include "volumegl.h"

namespace inviwo {

VolumeGL::VolumeGL()
    : VolumeRepresentation(),
    dimensions_(ivec3(128,128,128)) //TODO: use actual value
{
    initialize();
}

VolumeGL::VolumeGL(ivec3 dimensions)
    : VolumeRepresentation(),
    dimensions_(dimensions)
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
