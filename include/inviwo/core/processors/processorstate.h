#ifndef IVW_PROCESSORSTATE_H
#define IVW_PROCESSORSTATE_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

IVW_CORE_API enum CodeState {
    CODE_STATE_BROKEN,
    CODE_STATE_EXPERIMENTAL,
    CODE_STATE_STABLE
};

} // namespace

#endif // IVW_PROCESSORSTATE_H
