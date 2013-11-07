#ifndef IVW_MeshCreator_H
#define IVW_MeshCreator_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/datastructures/geometry/simplemeshram.h>
#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API MeshCreator : public Processor {
public:
    MeshCreator();
    ~MeshCreator();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    SimpleMeshRAM* createMesh();

protected:
    virtual void process();

private:
    GeometryOutport outport_;

    OptionPropertyString meshType_;
};

} // namespace

#endif // IVW_MeshCreator_H
