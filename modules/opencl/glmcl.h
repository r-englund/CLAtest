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

#ifndef __GLMCL_H__
#define __GLMCL_H__

#include <modules/opencl/cl.hpp>
#include <glm/glm.hpp>
#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/util/logdistributor.h>
#include <iostream>

namespace cl {


// Kernel argument specializations for vec3 types:
// int, uint, float

// int types
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int8>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int16>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int32>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::int64>& value);

// uint types
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint8>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint16>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint32>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::uint64>& value);


// float types
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float16>& value);
template <>
IVW_MODULE_OPENCL_API cl_int Kernel::setArg(cl_uint index, const glm::detail::tvec3<glm::detail::float32>& value);

} // namespace cl
#endif
