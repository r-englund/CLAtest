/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Rickard Englund
 *
 **********************************************************************/

#ifndef IVW_PYTONQTFUNCTIONS_H
#define IVW_PYTONQTFUNCTIONS_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <modules/python/pythoninterface/pymethod.h>


namespace inviwo{
    
    PyObject* py_loadWorkspace(PyObject* /*self*/, PyObject* /*args*/);


    class IVW_MODULE_PYTHONQT_API PyLoadNetwork : public PyMethod{
    public:
        std::string getName()const{return "loadWorkspace";}
        std::string getDesc()const{return "Load a new workspace into the network.";}
        virtual PyCFunction getFunc(){return py_loadWorkspace;}
    };

}

#endif