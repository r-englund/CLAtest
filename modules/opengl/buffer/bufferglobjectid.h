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

#ifndef IVW_BUFFER_GL_OBJECT_ID_H
#define IVW_BUFFER_GL_OBJECT_ID_H


#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/referencecounter.h>

namespace inviwo {
    
class BufferGLObjectId: public ReferenceCounter {
public:

    BufferGLObjectId(): object_(0) {};
    BufferGLObjectId(const GLuint id): object_(id) {};
    virtual ~BufferGLObjectId() { glDeleteBuffers(1, &object_);};

    GLuint getId() const { return object_; }
    GLuint& getId() { return object_; }

private:
    GLuint object_;
};

} // namespace

#endif // IVW_BUFFER_GL_OBJECT_ID_H
