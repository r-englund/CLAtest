#ifndef IVW_VOLUMERAMOPERATION_H
#define IVW_VOLUMERAMOPERATION_H

#include <inviwo/core/datastructures/volumeoperation.h>
#include <inviwo/core/datastructures/volumeram.h>

namespace inviwo {

    class VolumeRAM;

    class IVW_CORE_API VolumeRAMOperation : public VolumeOperation {
    public:
        VolumeRAMOperation(const VolumeRepresentation* in) : VolumeOperation(in), out_(NULL){}
        virtual ~VolumeRAMOperation(){}

        void setOutputVolume(VolumeRAM* vol) { out_=vol; }
        VolumeRAM* getOutputVolume() { return out_; }

    private:
        VolumeRAM* out_;
    };

} // namespace

#endif // IVW_VOLUMERAMOPERATION_H
