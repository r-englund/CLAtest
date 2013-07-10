#ifndef IVW_PYTHON_MODULE_H
#define IVW_PYTHON_MODULE_H

#include <modules/python/pythonmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

    class PyInviwo;
    class PythonEditorWidget;
class IVW_MODULE_PYTHON_API PythonModule : public InviwoModule {
    PyInviwo *pyInviwo_;
    PythonEditorWidget *pythonEditorWidget_;
public:
    /** 
     * \brief Creates the Python module
     *
     * Creates the Python inviwo-module. Creates the PythonEditorWidget and initializes the python interpreter. 
     * 
     */
    PythonModule();

    /** 
     * \brief Destructor for PythonModule
     *
     * Calls the destructor for the PythonEditorWidget and PyInviwo and then shut down the python interpreter. 
     * 
     * @return  DESCRIBE_ME
     */
    virtual ~PythonModule();
    
protected:
    /** 
     * \brief Add a path where python scripts looks for modules
     *
     * Add a path where python scripts looks for modules
     * 
     * @param const std::string & path Path to directory containing python modules
     */
    void addModulePath(const std::string& path);
};

} // namespace

#endif // IVW_OPENGL_QT_MODULE_H
