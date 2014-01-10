/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_LAYERCL_H
#define IVW_LAYERCL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API LayerCL : public LayerRepresentation {

public:
    LayerCL(uvec2 dimensions = uvec2(128,128), LayerType type = COLOR_LAYER, const DataFormatBase* format = DataFormatBase::get(), const void* data = NULL);
    LayerCL(const LayerCL& other);
    virtual ~LayerCL();
    virtual std::string getClassName() const { return "LayerCL"; }
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
    virtual void setDimension(uvec2 dimensions);
    virtual void resize(uvec2 dimensions);
    virtual bool copyAndResizeLayer(DataRepresentation* target) const;
    cl::ImageFormat getFormat() const { return layerFormat_;}
    cl::Image2D getEditable() { return *clImage2D_; }
    const cl::Image2D& get() const { return *const_cast<const cl::Image2D*>(clImage2D_); }

protected:
    cl::Image2D* clImage2D_;
    cl::ImageFormat layerFormat_;
};

} // namespace

namespace cl {

// Kernel argument specializations for LayerCL type 
// (enables calling cl::Queue::setArg with LayerCL)
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const inviwo::LayerCL& value);

} // namespace cl



#endif // IVW_LAYERCL_H
