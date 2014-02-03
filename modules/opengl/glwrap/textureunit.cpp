 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include "textureunit.h"

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/vectoroperations.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

bool TextureUnit::initialized_ = false;
unsigned int TextureUnit::numRequestedUnits_ = 0;
std::vector<bool> TextureUnit::requestedUnits_ = std::vector<bool>();

TextureUnit::TextureUnit() :
    requested_(false),
    unitEnum_(0),
    unitNumber_(0)
{
    if (!initialized_)
        initialize();
}

TextureUnit::~TextureUnit() {
    if (requested_) {
        requestedUnits_[(int)unitNumber_] = false;
        numRequestedUnits_--;
    }
}

void TextureUnit::initialize() {
    OpenGLModule* openGLModule = getTypeFromVector<OpenGLModule>(InviwoApplication::getRef().getModules());
    OpenGLCapabilities* openGLInfo = getTypeFromVector<OpenGLCapabilities>(openGLModule->getCapabilities());
    numRequestedUnits_ = 0;
    requestedUnits_ = std::vector<bool>(openGLInfo->getNumTexUnits());
    initialized_ = true;
}

GLint TextureUnit::getEnum() {
    if (!requested_)
        requestUnit();
    return unitEnum_;
}

GLint TextureUnit::getUnitNumber() {
    if (!requested_)
        requestUnit();
    return unitNumber_;
}

void TextureUnit::activate() {
    glActiveTexture(getEnum());
}

void TextureUnit::setZeroUnit() {
    glActiveTexture(GL_TEXTURE0);
}

void TextureUnit::requestUnit() {
    ivwAssert(numRequestedUnits_<requestedUnits_.size(), "Exceeding number of available texture units.");
    for (size_t i=0; i<requestedUnits_.size(); i++) {
        if (requestedUnits_[i] == false) {
            unitNumber_ = (GLint)i;
            unitEnum_ = GL_TEXTURE0 + unitNumber_;
            requestedUnits_[i] = true;
            numRequestedUnits_++;
            requested_ = true;
            return;
        }
    }
}

} // namespace
