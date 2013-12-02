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

#include <modules/opencl/cl.hpp>
#include <modules/opencl/inviwoopencl.h>
// An extended version of Khronos OpenCL wrapper
// As much as possible of a extensions should be kept in the cpp file 
// such that we can merge it with the one from Khronos

inline Device Device::getDefault(cl_int *)
{
    return inviwo::OpenCL::instance()->getDevice();
}

Context Context::getDefault(cl_int *) {
    return inviwo::OpenCL::instance()->getContext();
}

CommandQueue CommandQueue::getDefault(cl_int *) {
    return inviwo::OpenCL::instance()->getQueue();
}
