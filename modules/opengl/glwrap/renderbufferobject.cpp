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

#include "renderbufferobject.h"

namespace inviwo {

RenderBufferObject::RenderBufferObject() {
    glGenRenderbuffersEXT(1, &id_);
}

RenderBufferObject::~RenderBufferObject() {
    glDeleteRenderbuffersEXT(1, &id_);
}

void RenderBufferObject::activate() {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, id_);
}

void RenderBufferObject::deactivate() {
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

unsigned int RenderBufferObject::getID() const {
    return id_;
}

} // namespace
