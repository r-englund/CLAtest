#ifndef IVW_CUBEPROXYGEOMETRY_H
#define IVW_CUBEPROXYGEOMETRY_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/ports/volumeport.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/processorgl.h"
#include "modules/opengl/glwrap/shader.h"

namespace inviwo {

class CubeProxygeometry : public ProcessorGL {
public:
    CubeProxygeometry();
    ~CubeProxygeometry();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "CubeProxygeometry"; }
    virtual std::string getCategory() const  { return "Proxy Geometry"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    VolumePort inport_;

    Shader* shader_;
};

} // namespace

#endif // IVW_CUBEPROXYGEOMETRY_H
