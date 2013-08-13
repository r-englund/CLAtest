#ifndef IVW_PYINVIWO_H
#define IVW_PYINVIWO_H

#include <modules/python/pythonmoduledefine.h>

#ifdef _DEBUG
#undef _DEBUG //Prevent linking debug build of python
#include <Python.h>
#define _DEBUG 1
#else
#include <Python.h>
#endif

namespace inviwo {    
    
    class PyInviwo{
    public:

        /** 
         * \brief Initializes the Inviwos python modules
         *
         * Initializes the python modules (inviwo and inviwo_internal) used for communication between python and Inviwo. 
         */
        PyInviwo();
        /** 
         * \brief perform python test by running simple scripts
         *
         * Runs Python init scripts        
         */
        void performTest();
        /** 
         * \brief initialized python modules
         *
         * Initializes given python module. Can be used called from outside this python module.
         * 
         * @param char * moduleName name of the module
         * @param PyMethodDef * module static array of registered classes in a module
         */
        void initPyModule(char* moduleName, PyMethodDef* module);
        
    };

} //namespace

#endif // IVW_PYINVIWO_H
