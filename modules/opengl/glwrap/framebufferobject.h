#ifndef IVW_FRAMEBUFFEROBJECT_H
#define IVW_FRAMEBUFFEROBJECT_H

#include "inviwo/core/inviwo.h"
#include "../inviwoopengl.h"
#include "texture2d.h"

namespace inviwo {

class FrameBufferObject {

public:
    FrameBufferObject();
    ~FrameBufferObject();

    void activate();
    static void deactivate();

    void attachTexture(Texture2D* texture, GLenum attachementType=GL_COLOR_ATTACHMENT0_EXT);

    void checkStatus();

private:
    unsigned int id_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_FRAMEBUFFEROBJECT_H
