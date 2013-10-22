#ifndef IVW_VOLUMERAMOPERATIONEXECUTER_H
#define IVW_VOLUMERAMOPERATIONEXECUTER_H

#include <inviwo/core/datastructures/dataoperation.h>
#include <inviwo/core/datastructures/volume/volumeramsubset.h>

namespace inviwo {

    template<typename T, size_t B>
    IVW_CORE_API void executeOperationOnVolumeRAMPrecision(DataOperation* dop){
        VolumeRAMSubSet* volSubSetDop = dynamic_cast<VolumeRAMSubSet*>(dop);
        if (volSubSetDop){
            volSubSetDop->evaluate<T, B>();
        }
    };

} // namespace

#endif // IVW_VOLUMERAMOPERATIONEXECUTER_H
