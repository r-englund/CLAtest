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

#ifndef IVW_PYMEHTODINVIWO_H
#define IVW_PYMEHTODINVIWO_H

#include <modules/python/pythonmoduledefine.h>


#ifdef _DEBUG
    #undef _DEBUG //Prevent linking debug build of python
    #include <Python.h>
    #define _DEBUG 1
#else
    #include <Python.h>
#endif

#include <vector>


namespace inviwo {
    
  class IVW_MODULE_PYTHON_API PyMethod{
  protected:
      PyMethodDef def_;
  public:
      PyMethod();
      virtual char* getName() = 0;
      virtual char* getDesc() = 0;
      virtual PyCFunction getFunc() = 0;
      virtual int getFlags(){return METH_VARARGS;};

      PyMethodDef* getDef();

  };
} //namespace

#endif // IVW_PYMEHTODINVIWO_H
