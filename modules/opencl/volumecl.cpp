#include <modules/opencl/volumecl.h>

namespace inviwo {

VolumeCL::VolumeCL(DataFormatBase format, const void* data)
    :  VolumeRepresentation(uvec3(128,128,128), format), imageFormat_(dataFormatToCLImageFormat(format.getId())), image3D_(0)
{
    initialize(data);
}

VolumeCL::VolumeCL(uvec3 dimensions, DataFormatBase format, const void* data)
    : image3D_(0), VolumeRepresentation(dimensions, format), imageFormat_(dataFormatToCLImageFormat(format.getId()))
{
    initialize(data);
}

VolumeCL::~VolumeCL() {
    deinitialize(); 
}

void VolumeCL::initialize(const void* voxels) {
    image3D_ = new cl::Image3D(OpenCL::instance()->getContext(), CL_MEM_READ_WRITE, getFormat(), dimensions_.x, dimensions_.y, dimensions_.z);
    if (voxels != NULL) {
        OpenCL::instance()->getQueue().enqueueWriteImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, const_cast<void*>(voxels));
    }
    VolumeCL::initialize();
}

DataRepresentation* VolumeCL::clone() const {
    VolumeCL* newVolumeCL = new VolumeCL(dimensions_, getDataFormat());
    OpenCL::instance()->getQueue().enqueueCopyImage(*image3D_, (newVolumeCL->getVolume()), glm::svec3(0), glm::svec3(0), glm::svec3(dimensions_));
    return newVolumeCL;
}

void VolumeCL::deinitialize() {
    delete image3D_;
    image3D_ = 0;
}
void VolumeCL::upload( const void* data )
{
    OpenCL::instance()->getQueue().enqueueWriteImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, const_cast<void*>(data));
}

void VolumeCL::download( void* data ) const
{
    OpenCL::instance()->getQueue().enqueueReadImage(*image3D_, true, glm::svec3(0), glm::svec3(dimensions_), 0, 0, data);
}

} // namespace

namespace cl {

template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::VolumeCL& value)
{
    return setArg(index, value.getVolume());
}


} // namespace cl
