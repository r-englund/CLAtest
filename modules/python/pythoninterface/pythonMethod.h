#ifndef IVW_PYTHONMEHTODINVIWO_H
#define IVW_PYTHONMEHTODINVIWO_H

#include <modules/python/pythonmoduledefine.h>


#ifdef _DEBUG
    #undef _DEBUG //Prevent linking debug build of python
    #include <Python.h>
    #define _DEBUG 1
#else
    #include <Python.h>
#endif


#ifndef IVW_PYINVIWO_CPP
#error This file should only be included from pyinviwo.cpp
#endif


static Processor* getProcessor(std::string id){
    InviwoApplicationQt* appQt = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());  
    return appQt->getProcessorNetwork()->getProcessorByName(id);
}


namespace inviwo {
    
  class PythonMethod{
  protected:
      static std::vector<PythonMethod*> methods_;
      static std::vector<PythonMethod*> getMethods(){return methods_;}
  public:
      PythonMethod(){
          methods_.push_back(this);
      }
      virtual char* getName() = 0;
      virtual char* getDesc() = 0;
      virtual PyCFunction getFunc() = 0;
      virtual int getFlags(){return METH_VARARGS;};

      PyMethodDef getDef(){
          PyMethodDef d;
          d.ml_doc = getDesc();
          d.ml_flags = getFlags();
          d.ml_meth = getFunc();
          d.ml_name = getName();
          return d;
      }

  };
  std::vector<PythonMethod*> PythonMethod::methods_;
} //namespace

#endif // IVW_PYTHONMEHTODINVIWO_H
