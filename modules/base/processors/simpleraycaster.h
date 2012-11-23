#ifndef IVW_SIMPLERAYCASTER_H
#define IVW_SIMPLERAYCASTER_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/ports/volumeport.h"
#include "modules/opengl/inviwoopengl.h"
#include "modules/opengl/processorgl.h"
#include "modules/opengl/volumegl.h"
#include "modules/opengl/glwrap/shader.h"

namespace inviwo {

    class SimpleRaycaster : public ProcessorGL {
    public:
        SimpleRaycaster();
        ~SimpleRaycaster();
        virtual Processor* create() const;

        void initialize() throw (Exception);
        void deinitialize() throw (Exception);

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
