/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_INVIWO_H
#define IVW_INVIWO_H

#pragma warning(disable: 4290)

#define NOMINMAX

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <stdio.h>

#include <inviwo/core/common/inviwocommondefines.h>

// include glm
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/core/type.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dvec4 dvec4;
typedef glm::bvec2 bvec2;
typedef glm::bvec3 bvec3;
typedef glm::bvec4 bvec4;
typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::uvec4 uvec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::quat quat;

#include <inviwo/core/util/glmstreamoperators.h>

#include <inviwo/core/util/formats.h>

// error handling
#include <inviwo/core/util/assertion.h>
#include <inviwo/core/util/exception.h>
#include <inviwo/core/util/logdistributor.h>
#define IVW_UNUSED_PARAM(param) (void)param

#include <inviwo/core/io/serialization/ivwserialization.h>

//#define IVW_DEPRECATION_WARNINGS

#if defined(IVW_DEPRECATION_WARNINGS)
#define ivwDeprecatedMethod(newFunction) \
    std::cout << __FUNCTION__ << " is deprecated. Use " << newFunction << " instead." << std::endl; \
    std::cout << "(" << __FILE__ << " - Line " << __LINE__ << ")." << std::endl;
#else
#define ivwDeprecatedMethod(newFunction)
#endif

#endif // IVW_INVIWO_H
