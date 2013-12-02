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

#ifndef IVW_IMAGE_CL_RESIZER_H
#define IVW_IMAGE_CL_RESIZER_H

#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/openclmoduledefine.h>

namespace inviwo {

/** \class ImageCLResizer 
 * 
 * Helper class that resizes a 2D image. 
 * @note It will compile the OpenCL kernel the first time resize is called (may take some time).
 * @see ImageCL
 */
class IVW_MODULE_OPENCL_API ImageCLResizer {

public:

    /**
     * Resize image to given dimension. 
     * 
     * \param src (const cl::Image2D &) Image to get data from
     * \param dst (const cl::Image2D &) Image containing resized src image. Note that this should same dimension as resizeToDimension 
     * \param resizeToDimension (const ivec2 &) Size to resize to
     * \return (void)
     */
    static void resize(const cl::Image2D& src, const cl::Image2D& dst, const uvec2& resizeToDimension);

private:
    ImageCLResizer();
    ImageCLResizer(ImageCLResizer const&) {};
    void operator=(ImageCLResizer const&) {};
    /**
     * Kernel that takes two images as input. First image acts as source and second as destination.
     * 
     * \return (cl::Kernel*)
     */
    cl::Kernel* getResizeKernel() { return &resizeKernel_; }

    cl::Kernel resizeKernel_;


};


} // namespace

#endif // IVW_IMAGE_CL_RESIZER_H
