#ifndef IVW_ENTRYEXITPOINTS_H
#define IVW_ENTRYEXITPOINTS_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>
#include <inviwo/core/common/inviwoapplication.h>


namespace inviwo {

class IVW_MODULE_BASE_API EntryExitPoints : public ProcessorGL {
public:
    EntryExitPoints();
    ~EntryExitPoints();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    GeometryInport geometryPort_;
    ImageOutport entryPort_;
    ImageOutport exitPort_;

    CameraProperty camera_;
};

} // namespace

#endif // IVW_ENTRYEXITPOINTS_H
