#ifndef IVW_SHADERUTILS_H
#define IVW_SHADERUTILS_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/glwrap/shader.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/simplelightingproperty.h>
#include <inviwo/core/properties/simpleraycastingproperty.h>
#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

// SimpleLightingProperty
IVW_MODULE_OPENGL_API void addShaderDefines(Shader* shader, const SimpleLightingProperty& property);
IVW_MODULE_OPENGL_API void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property);

// CameraProperty
IVW_MODULE_OPENGL_API void addShaderDefines(Shader* shader, const CameraProperty& property);
IVW_MODULE_OPENGL_API void setShaderUniforms(Shader* shader, const CameraProperty& property);

// SimpleRaycastingProperty
IVW_MODULE_OPENGL_API void addShaderDefines(Shader* shader, const SimpleRaycastingProperty& property);
IVW_MODULE_OPENGL_API void setShaderUniforms(Shader* shader, const SimpleRaycastingProperty& property);

}  // namespace

#endif  // IVW_SHADERUTILS_H
