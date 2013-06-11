#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <modules/opengl/glwrap/texture3d.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeGL : public VolumeRepresentation {

public:
    VolumeGL(uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataFormatBase());
    VolumeGL(uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType);
    VolumeGL(void* texels, uvec3 dimensions = uvec3(128,128,128), DataFormatBase format = DataFormatBase());
    VolumeGL(void* texels, uvec3 dimensions, GLint format, GLint internalFormat, GLenum dataType);
    virtual ~VolumeGL();

    virtual void initialize();
    virtual void initialize(void* texels);
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    virtual GLint getFormat();
    virtual GLint getInternalFormat();
    virtual GLenum getDataType();
    virtual void bindTexture(GLenum texUnit) const;
    virtual void unbindTexture() const;

protected:
    GLint format_;
    GLint internalFormat_;
    GLenum dataType_;
    Texture3D* volumeTexture_;
};

} // namespace

#endif // IVW_VOLUMEGL_H
