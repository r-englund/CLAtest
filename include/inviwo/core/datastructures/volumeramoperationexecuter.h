#ifndef IVW_VOLUMERAMOPERATIONEXECUTER_H
#define IVW_VOLUMERAMOPERATIONEXECUTER_H

#include <inviwo/core/datastructures/volumeramsubset.h>

namespace inviwo {

    //class VolumeRAMSubSet;

    template<typename T, size_t B>
    static IVW_CORE_API inline void executeOperationOnVolumeRAMPrecision(DataOperation* dop){
        VolumeRAMSubSet* volSubSetDop = dynamic_cast<VolumeRAMSubSet*>(dop);
        if(volSubSetDop){
            volSubSetDop->evaluate<T, B>();
        }
    };

} // namespace

#endif // IVW_VOLUMERAMOPERATIONEXECUTER_H
