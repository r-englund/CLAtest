#ifndef IVW_PYINVIWOOBSERVER_H
#define IVW_PYINVIWOOBSERVER_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {
    class PyModule;
class IVW_MODULE_PYTHON_API PyInviwoObserver : public Observer{
public:    
    PyInviwoObserver();
    virtual ~PyInviwoObserver();

    virtual void onModuleRegistered(PyModule* module) = 0;
};

} // namespace

#endif // IVW_PYINVIWOOBSERVER_H

