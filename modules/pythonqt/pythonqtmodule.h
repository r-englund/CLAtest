#ifndef IVW_PYTHONQT_MODULE_H
#define IVW_PYTHONQT_MODULE_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {
class IVW_MODULE_PYTHONQT_API PythonQtModule : public InviwoModule {
public:
    PythonQtModule();
    virtual ~PythonQtModule();
};

} // namespace

#endif // IVW_PYTHONQT_MODULE_H
