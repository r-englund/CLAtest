#ifndef IVW_MINMAXPROCESSOR_H
#define IVW_MINMAXPROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/processorgl.h>

#include <modules/opengl/glwrap/shader.h>

#include <inviwo/core/io/imageloader.h>

#include <inviwo/core/datastructures/imageram.h>
#include <inviwo/core/datastructures/imagedisk.h>
namespace inviwo {

class IVW_MODULE_BASE_API SnapshotProcessor : public ProcessorGL {
public:
    SnapshotProcessor();
    ~SnapshotProcessor();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "SnapshotProcessor"; }
    virtual std::string getCategory() const  { return "Dummy Processors"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

protected:
    virtual void process();

private:
    void buttonAction();
    FileProperty imageFileName_;
    ButtonProperty buttonPropperty_;
    //SnapshotProperty snapshotProperty_;

	ImageInport inport0_;
    ImageOutport outport_;
    Shader* shader_;
};

} // namespace

#endif // IVW_MINMAXPROCESSOR_H
