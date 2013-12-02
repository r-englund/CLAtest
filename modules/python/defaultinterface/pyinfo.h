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

#ifndef IVW_INFOMEHTODINVIWO_H
#define IVW_INFOMEHTODINVIWO_H


#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

PyObject* py_info(PyObject* /*self*/, PyObject* /*args*/);

namespace inviwo {
    class PyInfoMethod : public PyMethod{
    public:
        char *getName(){return "info";}
        char *getDesc(){return "info()\tPrints documentation of the module's functions.";}
        virtual PyCFunction getFunc(){return py_info;}

        static void printAllDescriptions(){
            for(int i = 0;i<methods_.size();i++){
                std::string msg = "print \"";
                msg += methods_[i]->getDesc();
                msg += "\" ";
                PyRun_SimpleString(msg.c_str());
                PyRun_SimpleString("print \" \"");
            }
        }

    };

} //namespace


#endif // IVW_INFOMEHTODINVIWO_H


