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

#ifndef IVW_PYCANVASMEHTODSINVIWO_H
#define IVW_PYCANVASMEHTODSINVIWO_H



#include <modules/python/pythonmoduledefine.h>

#include "../pythoninterface/pymethod.h"

namespace inviwo {
PyObject* py_canvascount(PyObject* /*self*/, PyObject* /*args*/);
PyObject* py_resizecanvas(PyObject* /*self*/, PyObject* /*args*/);


    class IVW_MODULE_PYTHON_API PyCanvasCountMethod : public PyMethod{
    public:
        std::string getName(){return "canvasCount";}
        std::string getDesc(){return "Returns the number of canvases in the current network.";}
        virtual PyCFunction getFunc(){return py_canvascount;}
    };

    class IVW_MODULE_PYTHON_API PyResizeCanvasMethod : public PyMethod{
    public:
    std::string getName(){return "resizeCanvas";}
    std::string getDesc(){return "Resizes all the canvases in the network to the given size. Canvas can either be given using a canvas index (starting at 0) or a canvas ID string ";}
    virtual PyCFunction getFunc(){return py_resizecanvas;}
    };

} //namespace


#endif // IVW_PYCANVASMEHTODSINVIWO_H


