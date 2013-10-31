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
