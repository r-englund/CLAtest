#ifndef IVW_SIMPLERAYCASTER_H
#define IVW_SIMPLERAYCASTER_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/ports/volumeport.h"
#include "../../inviwo/modules/opengl/inviwoopengl.h"
#include "../../inviwo/modules/opengl/processorgl.h"
#include "../../inviwo/modules/opengl/volumegl.h"
#include "../../inviwo/modules/opengl/glwrap/shader.h"

namespace inviwo {

    class SimpleRaycaster : public ProcessorGL {
    public:
        SimpleRaycaster();
        ~SimpleRaycaster();
        virtual Processor* create() const;

        void initialize();
        void deinitialize();

        virtual std::string getClassName() const { return "SimpleRaycaster"; }
        virtual std::string getCategory() const  { return "Ray Caster"; }
        virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

    protected:
        virtual void process();

    private:
        VolumePort volumePort_;
        ImagePort entryPort_;
        ImagePort exitPort_;
        ImagePort outport_;

        Shader* shader_;
    };

} // namespace

#endif // IVW_SIMPLERAYCASTER_H
