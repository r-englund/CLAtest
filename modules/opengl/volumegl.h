#ifndef IVW_VOLUMEGL_H
#define IVW_VOLUMEGL_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/volumerepresentation.h"
#include "glwrap/texture3d.h"

namespace inviwo {

    class VolumeGL : public VolumeRepresentation {

    public:
        VolumeGL();
        VolumeGL(ivec3 dimensions);
        virtual ~VolumeGL();

        void initialize();
        void deinitialize();

        void bindTexture(GLenum texUnit);

    private:
        ivec3 dimensions_;
        Texture3D* volumeTexture_;
    };

} // namespace

#endif // IVW_VOLUMEGL_H
