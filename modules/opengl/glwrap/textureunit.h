#ifndef IVW_TEXTUREUNIT_H
#define IVW_TEXTUREUNIT_H


#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/resourceinfo.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API TextureUnit {
public:

    TextureUnit();
    virtual ~TextureUnit();

    GLint getEnum();
    GLint getUnitNumber();
    void requestUnit();
    void activate();

    static void setZeroUnit();

private:
    static bool initialized_;
    static unsigned int numRequestedUnits_;
    static std::vector<bool> requestedUnits_;

    static void initialize();

    bool requested_;
    GLint unitEnum_;
    GLint unitNumber_;
};

} // namespace

#endif // IVW_TEXTUREUNIT_H
