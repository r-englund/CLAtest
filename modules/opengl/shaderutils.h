#ifndef IVW_SHADERUTILS_H
#define IVW_SHADERUTILS_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/glwrap/shader.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/simplelightingproperty.h>
#include <inviwo/core/properties/simpleraycastingproperty.h>
#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

namespace util {

// SimpleLightingProperty
IVW_MODULE_OPENGL_API void glAddShaderDefines(Shader* shader,
                                              const SimpleLightingProperty& property);
IVW_MODULE_OPENGL_API void glSetShaderUniforms(Shader* shader,
                                               const SimpleLightingProperty& property);

// CameraProperty
IVW_MODULE_OPENGL_API void glAddShaderDefines(Shader* shader, const CameraProperty& property);
IVW_MODULE_OPENGL_API void glSetShaderUniforms(Shader* shader, const CameraProperty& property);

// SimpleRaycastingProperty
IVW_MODULE_OPENGL_API void glAddShaderDefines(Shader* shader,
                                              const SimpleRaycastingProperty& property);
IVW_MODULE_OPENGL_API void glSetShaderUniforms(Shader* shader,
                                               const SimpleRaycastingProperty& property);

}  // namspace util

}  // namespace

#endif  // IVW_SHADERUTILS_H
