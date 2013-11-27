#ifndef IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H
#define IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H

#include <modules/python/pythonmoduledefine.h>

namespace inviwo {
class PythonExecutionOutputObeserver{
public:
    enum OutputType
    {
        standard,
        error
    };
    virtual void onPyhonExecutionOutput(std::string msg,OutputType outputType) = 0;
    PythonExecutionOutputObeserver();

    static void pyhonExecutionOutputEvent(std::string msg,OutputType outputType);
    static std::vector<PythonExecutionOutputObeserver*> observers_;
};

} // namespace

#endif // IVW_PYTHONEXECUTIONOUTPUTOBESERVER_MODULE_H
