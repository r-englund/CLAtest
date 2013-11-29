#ifndef IVW_OPENGLQTCAPABILITIES_H
#define IVW_OPENGLQTCAPABILITIES_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/util/capabilities.h>
#include <modules/opengl/openglcapabilities.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API OpenGLQtCapabilities : public Capabilities  {
public:    
    OpenGLQtCapabilities();
    virtual ~OpenGLQtCapabilities();
    void printInfo();
    virtual void retrieveStaticInfo() {};
    virtual void retrieveDynamicInfo() {};
    std::vector<int> getGLVersion();
};

} // namespace

#endif // IVW_OPENGLQTCAPABILITIES_H
