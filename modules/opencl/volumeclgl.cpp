#include <modules/opencl/volumeclgl.h>
#include <modules/opencl/imageclresizer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

VolumeCLGL::VolumeCLGL(const DataFormatBase* format, const Texture3D* data)
: VolumeRepresentation(data != NULL ? data->getDimension(): uvec3(64), format), image3D_(0), texture_(data) 
{
    if(data) {
        initialize(data);
    }
    
}

VolumeCLGL::VolumeCLGL(const uvec3& dimensions, const DataFormatBase* format, const Texture3D* data)
    : VolumeRepresentation(dimensions, format), texture_(data)
{
    initialize(data);
}

VolumeCLGL::~VolumeCLGL() { 
    deinitialize(); 
}

void VolumeCLGL::initialize(const Texture3D* texture) {
    ivwAssert(texture != 0, "Cannot initialize with null OpenGL texture");
    image3D_ = new cl::Image3DGL(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, GL_TEXTURE_3D, 0, texture->getID());
    VolumeCLGL::initialize();
}

DataRepresentation* VolumeCLGL::clone() const {
    VolumeCLGL* newVolumeCLGL = new VolumeCLGL(dimensions_, getDataFormat(), NULL);
    OpenCL::instance()->getQueue().enqueueCopyImage(*image3D_, (newVolumeCLGL->getVolume()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_));
    return newVolumeCLGL;
}

void VolumeCLGL::deinitialize() {
    delete image3D_;
    image3D_ = 0; 
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCLGL& value)
{
    return setArg(index, value.getVolume());
}


} // namespace cl
