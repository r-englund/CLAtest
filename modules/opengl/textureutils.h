/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_TEXTUREUTILS_H
#define IVW_TEXTUREUTILS_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/portgroup.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

class BufferObjectArray;
class Shader;

namespace util {

IVW_MODULE_OPENGL_API void glActivateTarget(ImageOutport& outport);
IVW_MODULE_OPENGL_API void glDeactivateCurrentTarget();

IVW_MODULE_OPENGL_API void glActivateAndClearTarget(ImageOutport& outport);
IVW_MODULE_OPENGL_API void glActivateAndClearTargets(PortGroup& portGroup);
IVW_MODULE_OPENGL_API void glClearCurrentTarget();

IVW_MODULE_OPENGL_API void glUpdateAndActivateTarget(ImageOutport& outport, ImageInport& inport);


// Bind textures with glenum
IVW_MODULE_OPENGL_API void bindTextures(const Image* image, bool color, bool depth, bool picking,
                                        GLenum colorTexUnit, GLenum depthTexUnit,
                                        GLenum pickingTexUnit);

IVW_MODULE_OPENGL_API void glBindColorTexture(const ImageInport& inport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindColorTexture(const ImageOutport& outport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindDepthTexture(const ImageInport& inport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindDepthTexture(const ImageOutport& outport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindPickingTexture(const ImageInport& inport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindPickingTexture(const ImageOutport& outport, GLenum texUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const Image* image, GLenum colorTexUnit,
                                          GLenum depthTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageInport& inport, GLenum colorTexUnit,
                                          GLenum depthTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageOutport& outport, GLenum colorTexUnit,
                                          GLenum depthTexUnit);

IVW_MODULE_OPENGL_API void glBindTextures(const Image* image, GLenum colorTexUnit,
                                          GLenum depthTexUnit, GLenum pickingTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageInport& inport, GLenum colorTexUnit,
                                          GLenum depthTexUnit, GLenum pickingTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageOutport& outport, GLenum colorTexUnit,
                                          GLenum depthTexUnit, GLenum pickingTexUnit);

// Bind textures with TextureUnit
IVW_MODULE_OPENGL_API void glBindColorTexture(const ImageInport& inport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindColorTexture(const ImageOutport& outport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindDepthTexture(const ImageInport& inport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindDepthTexture(const ImageOutport& outport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindPickingTexture(const ImageInport& inport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindPickingTexture(const ImageOutport& outport, const TextureUnit& texUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const Image* image, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageInport& inport, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageOutport& outport, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit);

IVW_MODULE_OPENGL_API void glBindTextures(const Image* image, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit, const TextureUnit& pickingTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageInport& inport, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit, const TextureUnit& pickingTexUnit);
IVW_MODULE_OPENGL_API void glBindTextures(const ImageOutport& outport, const TextureUnit& colorTexUnit,
                                          const TextureUnit& depthTexUnit, const TextureUnit& pickingTexUnit);



// Unbind textures
IVW_MODULE_OPENGL_API void unbindTextures(const Image* image, bool color, bool depth, bool picking);

IVW_MODULE_OPENGL_API void glUnbindColorTexture(const ImageInport& inport);
IVW_MODULE_OPENGL_API void glUnbindColorTexture(const ImageOutport& outport);
IVW_MODULE_OPENGL_API void glUnbindDepthTexture(const ImageInport& inport);
IVW_MODULE_OPENGL_API void glUnbindDepthTexture(const ImageOutport& outport);
IVW_MODULE_OPENGL_API void glUnbindPickingTexture(const ImageInport& inport);
IVW_MODULE_OPENGL_API void glUnbindPickingTexture(const ImageOutport& outport);

IVW_MODULE_OPENGL_API void glUnbindTextures(const Image* image);
IVW_MODULE_OPENGL_API void glUnbindTextures(const ImageInport& inport);
IVW_MODULE_OPENGL_API void glUnbindTextures(const ImageOutport& outport);

// convenience texture bindings
IVW_MODULE_OPENGL_API void glBindTexture(const TransferFunctionProperty& tf,
                                         const TextureUnit& texUnit);

IVW_MODULE_OPENGL_API void glBindTexture(const Volume* volume, const TextureUnit& texUnit);

IVW_MODULE_OPENGL_API void glBindTexture(const VolumeInport& inport, const TextureUnit& texUnit);

// Shader defines.
IVW_MODULE_OPENGL_API void glSetTextureParameters(const Image* image, Shader* shader,
                                                  const std::string samplerID);
IVW_MODULE_OPENGL_API void glSetTextureParameters(const ImageInport& inport, Shader* shader,
                                                  const std::string samplerID);
IVW_MODULE_OPENGL_API void glSetTextureParameters(const ImageOutport& outport, Shader* shader,
                                                  const std::string samplerID);

// Draw image plane.
IVW_MODULE_OPENGL_API BufferObjectArray* glEnableImagePlaneRect();

IVW_MODULE_OPENGL_API void glDisableImagePlaneRect(BufferObjectArray*);

IVW_MODULE_OPENGL_API void glSingleDrawImagePlaneRect();

IVW_MODULE_OPENGL_API void glMultiDrawImagePlaneRect(int instances);
}

}  // namespace

#endif  // IVW_TEXTUREUTILS_H
