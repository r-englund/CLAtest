#ifndef IVW_PYTHON_SCRIPT_H
#define IVW_PYTHON_SCRIPT_H

#ifdef _DEBUG
    #undef _DEBUG //Prevent linking debug build of python
        #include <Python.h>
    #define _DEBUG 1
#else
    #include <Python.h>
#endif

#include <string>

#include <modules/python/pythonmoduledefine.h>

namespace inviwo {

/**
 * Stores a Python script, runs it and checks for errors.
 * Additionally, the script source can be compiled to byte code,
 * resulting in a faster script execution and clearer error messages.
 *
 * @note The caller has to make sure that the Python interpreter is initialized
 *  during all interactions with this class.
 *
 * @note Convenient loading of Python scripts with search path handling
 *  is provided by the PythonModule class.
 */
class IVW_MODULE_PYTHON_API PythonScript {

public:
    PythonScript();

    /**
     * Frees the stored byte code. Make sure that the
     * Python interpreter is still initialized
     * when deleting the script.
     */
    ~PythonScript();

    /**
     * Loads a Python script from file and compiles it to byte code by default.
     *
     * @return true, if the script has been loaded successfully.
     *  Note: Does not check the script for validity.
     */
    bool load(const std::string& filename, bool compile = true);

    /**
     * Assigns the Python script source, replacing the current source.
     */
    void setSource(const std::string& source);

    /**
     * Returns the script's source.
     */
    std::string getSource() const;

    /**
     * Compiles the script source to byte code, which speeds up script execution
     * and is generally recommended, since it also produces more clear error messages.
     *
     * If an error occurs, the error message is stored and can be retrieved through getLog().
     *
     * @param logErrors if true, error messages are also passed to the logger.
     *  The internal log buffer is not affected by this parameter.
     *
     * @return true, if script compilation has been successful
     */
    bool compile(bool logErrors = true);

    /**
     * Runs the script source as string or runs the byte code,
     * if the script has been compiled.
     *
     * If an error occurs, the error message is stored and can be retrieved through getLog().
     *
     * @param logErrors if true, error messages are also passed to the logger.
     *  The internal log buffer is not affected by this parameter.
     *
     * @return true, if script execution has been successful
     */
    bool run(bool logErrors = true);

    /**
     * Returns the error that has occured during the last operation (compilation of exection).
     * If the last operation has been successful, an empty string is returned.
     */
    std::string getLog() const;

private:
    bool checkCompileError(bool logErrors = true);
    bool checkRuntimeError(bool logErrors = true);

    std::string     source_;
    PyObject*       byteCode_;
    bool            compiled_;
    std::string     log_;
    //int             errorLine_;
    //int             errorCol_;
};

}

#endif // VRN_PYTHONSCRIPT_H
