#ifndef IVW_INVIWO_H
#define IVW_INVIWO_H

#pragma warning(disable: 4290)

#define NOMINMAX

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <stdio.h>

#include <inviwo/core/common/inviwocommondefines.h>

#ifdef WIN32
#include <windows.h>
#endif

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
typedef glm::bvec3 bvec4;
typedef glm::uvec2 uvec2;
typedef glm::uvec3 uvec3;
typedef glm::uvec4 uvec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::quat quat;

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
