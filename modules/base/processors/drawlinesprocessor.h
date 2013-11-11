#ifndef IVW_DRAWLINESPROCESSOR_H
#define IVW_DRAWLINESPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {

class IVW_MODULE_BASE_API DrawLinesProcessor : public CompositeProcessorGL {
public:
    DrawLinesProcessor();
    ~DrawLinesProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return true; }

protected:
    void addPoint(vec2);
    void clearPoints();

    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

    Mesh* lines_;
    MeshGLRenderer* lineRenderer_;
};

} // namespace

#endif // IVW_DRAWLINESPROCESSOR_H
