#ifndef IVW_IMAGECLGL_H
#define IVW_IMAGECLGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>
#include <modules/opengl/glwrap/texture2d.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCLGL : public ImageRepresentation {

public:
    ImageCLGL(DataFormatBase format = DataFormatBase(), const Texture2D* data = NULL);
    ImageCLGL(uvec2 dimensions, DataFormatBase format, const Texture2D* data);
    virtual ~ImageCLGL();
    virtual std::string getClassName() const { return "ImageCLGL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const Texture2D* texture);

    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeImage(DataRepresentation* target);
    cl::Image2DGL getEditableImage() { return *image2D_; }
    const cl::Image2DGL& getImage() const { return *(image2D_); }
    const Texture2D* getTexture() const { return texture_; }

    void aquireGLObject(std::vector<cl::Event>* syncEvents = NULL) const {
        std::vector<cl::Memory> syncImages(1, *image2D_); 
        OpenCL::getInstance()->getQueue().enqueueAcquireGLObjects(&syncImages, syncEvents);
    }
    void releaseGLObject(std::vector<cl::Event>* syncEvents = NULL, cl::Event* event= NULL) const {
        std::vector<cl::Memory> syncImages(1, *image2D_); 
        OpenCL::getInstance()->getQueue().enqueueReleaseGLObjects(&syncImages, syncEvents, event);
    }

protected:
    cl::Image2DGL* image2D_;
    const Texture2D* texture_;
};

} // namespace

namespace cl {

// Kernel argument specializations for ImageCLGL type 
// (enables calling cl::Queue::setArg with ImageCLGL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCLGL& value);

} // namespace cl



#endif // IVW_IMAGECLGL_H
