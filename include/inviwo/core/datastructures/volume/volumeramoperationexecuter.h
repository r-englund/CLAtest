#ifndef IVW_VOLUMERAMOPERATIONEXECUTER_H
#define IVW_VOLUMERAMOPERATIONEXECUTER_H

#include <inviwo/core/datastructures/dataoperation.h>
#include <inviwo/core/datastructures/volume/volumeramslice.h>
#include <inviwo/core/datastructures/volume/volumeramsubset.h>

namespace inviwo {

//TODO: Make operation factory
template<typename T, size_t B>
IVW_CORE_API void executeOperationOnVolumeRAMPrecision(DataOperation* dop){
    VolumeRAMSlice* volSliceDop = dynamic_cast<VolumeRAMSlice*>(dop);
    if (volSliceDop){
        volSliceDop->evaluate<T, B>();
        return;
    }
    VolumeRAMSubSet* volSubSetDop = dynamic_cast<VolumeRAMSubSet*>(dop);
    if (volSubSetDop){
        volSubSetDop->evaluate<T, B>();
        return;
    }
};

} // namespace

#endif // IVW_VOLUMERAMOPERATIONEXECUTER_H
