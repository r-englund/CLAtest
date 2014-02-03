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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_FRAMEBUFFEROBJECT_H
#define IVW_FRAMEBUFFEROBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include "texture2d.h"
#include "texture3d.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API FrameBufferObject {

public:
    FrameBufferObject();
    ~FrameBufferObject();

    void activate();
    void defineDrawBuffers();
    static void deactivate();

    //For attaching a 2D Texture
    void attachTexture(Texture2D* texture, GLenum attachementID);
    GLenum attachColorTexture(Texture2D* texture);
    GLenum attachColorTexture(Texture2D* texture, int attachmentNumber, bool attachFromRear = false);

    //For attaching a 3D Texture
    void attachTexture(Texture3D* texture, GLenum attachementID);
    GLenum attachColorTexture(Texture3D* texture);
    GLenum attachColorTexture(Texture3D* texture, int attachmentNumber, bool attachFromRear = false);

    //For attaching a layer of a 3D Texture
    void attachTextureLayer(Texture3D* texture, GLenum attachementID, int layer);
    GLenum attachColorTextureLayer(Texture3D* texture, int layer);
    GLenum attachColorTextureLayer(Texture3D* texture, int attachmentNumber, int layer, bool attachFromRear = false);

    void detachTexture(GLenum attachementID);
    void detachAllTextures();

    unsigned int getID() const;
    const GLenum* getDrawBuffers() const;
    int getMaxColorAttachments() const;

    bool hasColorAttachment() const;
    bool hasDepthAttachment() const;
    bool hasStencilAttachment() const;

    void checkStatus();

    void setRead_Blit(bool set=true) const;
    void setDraw_Blit(bool set=true);

protected:
    void performAttachTexture(GLenum attachementID);
    bool performAttachColorTexture(GLenum& outAttachNumber);
    bool performAttachColorTexture(GLenum& outAttachNumber, int attachmentNumber, bool attachFromRear = false);

private:
    unsigned int id_;
    bool hasDepthAttachment_;
    bool hasStencilAttachment_;

    GLenum* drawBuffers_;
    int maxColorAttachements_;

    const static GLenum colorAttachmentEnums_[];
};

} // namespace

#endif // IVW_FRAMEBUFFEROBJECT_H
