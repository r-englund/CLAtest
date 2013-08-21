#ifndef IVW_IMAGECL_H
#define IVW_IMAGECL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/util/typetostring.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API ImageCL : public ImageRepresentation {

public:
    ImageCL(uvec2 dimensions = uvec2(128,128), ImageType type = COLOR_DEPTH, DataFormatBase format = DataFormatBase(), const void* data = NULL);
    virtual ~ImageCL();
    virtual std::string getClassName() const { return "ImageCL"; }
    virtual void initialize(){};
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;
    
    void initialize(const void* texels);
    void upload(const void* data);
    /**
     * Download data to preallocated memory.
     * 
     * @param data (void *) Preallocated pointer that will contain data after function returns.
     * @return (void)
     */
    void download(void* data) const; 
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeImage(DataRepresentation* target);
    cl::ImageFormat getFormat() const { return imageFormat_;}
    cl::Image2D getEditableImage() { return *image2D_; }
    const cl::Image2D& getImage() const { return *const_cast<const cl::Image2D*>(image2D_); }

protected:
    cl::Image2D* image2D_;
    cl::ImageFormat imageFormat_;
};

} // namespace

namespace cl {

// Kernel argument specializations for ImageCL type 
// (enables calling cl::Queue::setArg with ImageCL)
template <>
cl_int Kernel::setArg(cl_uint index, const inviwo::ImageCL& value);

} // namespace cl



#endif // IVW_IMAGECL_H
