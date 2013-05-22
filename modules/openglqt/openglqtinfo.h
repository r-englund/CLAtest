#ifndef IVW_OPENGL_QT_INFO_H
#define IVW_OPENGL_QT_INFO_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/util/resourceinfo.h>

namespace inviwo {

class IVW_MODULE_OPENGL_QT_API OpenglQtInfo : public ResourceInfo {
public:    
    OpenglQtInfo();
    virtual ~OpenglQtInfo();
    void printInfo();
    virtual void retrieveStaticInfo() {};
    virtual void retrieveDynamicInfo() {};
};

} // namespace

#endif // IVW_OPENGL_QT_INFO_H
