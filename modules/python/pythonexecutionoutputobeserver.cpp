#include <modules/python/pythonmodule.h>

#include "pythonexecutionoutputobeserver.h"

namespace inviwo {



PythonExecutionOutputObeserver::PythonExecutionOutputObeserver(){
    observers_.push_back(this);
}

void PythonExecutionOutputObeserver::pyhonExecutionOutputEvent(std::string msg,OutputType outputType){
    std::vector<PythonExecutionOutputObeserver*>::iterator it;
    for(it = observers_.begin();it != observers_.end();++it){
        (*it)->onPyhonExecutionOutput(msg,outputType);
    }
}

std::vector<PythonExecutionOutputObeserver*> PythonExecutionOutputObeserver::observers_;


} // namespace
