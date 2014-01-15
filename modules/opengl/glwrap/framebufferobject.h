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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_FRAMEBUFFEROBJECT_H
#define IVW_FRAMEBUFFEROBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include "texture2d.h"
#include "texture3d.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API FrameBufferObject {

public:
    FrameBufferObject();
    ~FrameBufferObject();

    void activate();
    static void deactivate();

    void attachTexture(Texture2D* texture, GLenum attachementID);
    GLenum attachColorTexture(Texture2D* texture);
    GLenum attachColorTexture(Texture2D* texture, int attachmentNumber, bool attachFromRear = false);

    void attachTexture(Texture3D* texture, GLenum attachementID, int layer = 0);
    GLenum attachColorTexture(Texture3D* texture, int layer = 0);
    GLenum attachColorTexture(Texture3D* texture, int attachmentNumber, int layer, bool attachFromRear);

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
    void attachTexture(GLenum attachementID);
    bool attachColorTexture(GLenum& outAttachNumber);
    bool attachColorTexture(GLenum& outAttachNumber, int attachmentNumber, bool attachFromRear = false);

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
