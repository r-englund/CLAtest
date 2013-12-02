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

#include <inviwo/core/common/inviwocommondefines.h>
#include <inviwo/core/util/assertion.h>
#include <inviwo/core/util/logdistributor.h>
#include <modules/opencl/image/imageclresizer.h>

namespace inviwo {


ImageCLResizer::ImageCLResizer() {

    cl::Program program = OpenCL::buildProgram(IVW_DIR+"modules/opencl/cl/img_resize.cl");

    resizeKernel_ = cl::Kernel(program, "resizeImage");
}

 void ImageCLResizer::resize(const cl::Image2D& src, const cl::Image2D& dst, const uvec2& resizeToDimension)
 {
    static ImageCLResizer instance;

    instance.getResizeKernel()->setArg(0, src);
    instance.resizeKernel_.setArg(1, dst);
    cl::Event event;
    OpenCL::instance()->getQueue().enqueueNDRangeKernel(instance.resizeKernel_, cl::NullRange, cl::NDRange(resizeToDimension[0], resizeToDimension[1]),
        cl::NullRange, NULL, &event);
    
    event.wait();
    #if IVW_PROFILING
    LogInfo("Image resizing from (" << src.getImageInfo<CL_IMAGE_WIDTH>() << ", " << src.getImageInfo<CL_IMAGE_HEIGHT>() << ") to (" << resizeToDimension.x << ", " << resizeToDimension.y << ") in " << event.getElapsedTime() << " ms");
    #endif
 }
} // namespace
