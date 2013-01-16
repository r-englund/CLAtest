#ifndef IVW_INVIWOCORE_H
#define IVW_INVIWOCORE_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/inviwomodule.h"

namespace inviwo {

class IVW_CORE_API InviwoCore : public InviwoModule {

public:
    InviwoCore();

protected:
    static const std::string loggerCat_;
};

} // namespace

#endif // IVW_INVIWOCORE_H
