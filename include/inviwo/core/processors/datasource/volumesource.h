#ifndef IVW_VOLUMESOURCE_H
#define IVW_VOLUMESOURCE_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/processors/processor.h"
#include "inviwo/core/ports/volumeport.h"

namespace inviwo {

    class VolumeSource : public Processor {
    public:
        VolumeSource();
        virtual Processor* create() const;

        virtual void initialize();
        virtual void deinitialize();

        virtual std::string getClassName() const { return "VolumeSource"; }
        virtual std::string getCategory() const  { return "Data Source Processor"; }
        virtual CodeState getCodeState() const   { return CODE_STATE_STABLE; }

    protected:
        virtual void process();

    private:
        VolumePort volumePort_;
    };

} // namespace

#endif // IVW_VOLUMESOURCE_H
