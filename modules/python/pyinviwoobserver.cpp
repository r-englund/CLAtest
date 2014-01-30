#include "pyinviwoobserver.h"
#include "pyinviwo.h"

namespace inviwo {

    PyInviwoObserver::PyInviwoObserver(){
        PyInviwo::getPtr()->addObserver(this);
    }

    PyInviwoObserver::~PyInviwoObserver(){
        PyInviwo::getPtr()->removeObserver(this);
    }


} // namespace

