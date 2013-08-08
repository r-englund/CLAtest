#ifndef IVW_PYINVIWO_H
#define IVW_PYINVIWO_H

#include <modules/python/pythonmoduledefine.h>

namespace inviwo {
    
    class PyInviwo{
    public:

        /** 
         * \brief Initializes the Inviwos python modules
         *
         * Initializes the python modules (inviwo and inviwo_internal) used for communication between python and Inviwo. 
         */
        PyInviwo();
        
    };

} //namespace

#endif // IVW_PYINVIWO_H
