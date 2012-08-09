#include "volumegl.h"

namespace inviwo {

    VolumeGL::VolumeGL()
        : VolumeRepresentation(),
        dimensions_(ivec3(256,256,256)) //TODO: use actual value
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

    void VolumeGL::initialize() {
        volumeTexture_ = new Texture3D(dimensions_, GL_ALPHA, GL_ALPHA, GL_UNSIGNED_BYTE, GL_LINEAR);
        //volumeTexture_->loadTexture("D:/inviwo/data/volumes/nucleon.raw", dimensions_=ivec3(41,41,41));
        volumeTexture_->loadTexture("D:/inviwo/data/volumes/hydrogenatom.raw", dimensions_=ivec3(128,128,128));
        //volumeTexture_->loadTexture("D:/inviwo/data/volumes/neghip.raw", dimensions_=ivec3(64,64,64));
        //volumeTexture_->loadTexture("D:/inviwo/data/volumes/marschnerlobb.raw", dimensions_=ivec3(41,41,41));
        //volumeTexture_->loadTexture("D:/inviwo/data/volumes/aneurysm.raw", dimensions_=ivec3(256,256,256));
    }

    void VolumeGL::deinitialize() {
        delete volumeTexture_;
    }

    void VolumeGL::bindTexture(GLenum texUnit) {
        glActiveTexture(texUnit);
        volumeTexture_->bind();
    }

} // namespace
