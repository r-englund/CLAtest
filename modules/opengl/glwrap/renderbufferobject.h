/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_RENDERBUFFEROBJECT_H
#define IVW_RENDERBUFFEROBJECT_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API RenderBufferObject {

public:
    RenderBufferObject();
    ~RenderBufferObject();

    void activate();
    static void deactivate();

    unsigned int getID() const;

private:
    unsigned int id_;
};

} // namespace

#endif // IVW_RENDERBUFFEROBJECT_H
