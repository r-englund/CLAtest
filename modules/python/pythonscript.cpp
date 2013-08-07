/*
    Have a look at the script.cpp sample and at
    http://docs.python.org/api/api.html
    for more information about binding C/C++ stuff to python
*/

#include "pythonscript.h"

#include <inviwo\core\util\assertion.h>
#include <inviwo\core\util\stringconversion.h>
#include <inviwo\core\util\filedirectory.h>

#include <traceback.h>
#include <frameobject.h>

namespace inviwo {

PythonScript::PythonScript()
    : source_("")
    , byteCode_(0)
    , compiled_(false)
    , errorLine_(-1)
    , errorCol_(-1)
    , printErrorsToStdOut_(false)
{}

PythonScript::~PythonScript() {
    Py_XDECREF(byteCode_);
}

std::string PythonScript::getLog() const {
    return log_;
}

bool PythonScript::load(const std::string& filename, bool pCompile) {
    if(!URLParser::fileExists(filename)){
        LogError("File could not be found: " << filename );
        return false;
    }
    
    std::ifstream file(filename.c_str());
  
    if (!file.is_open()){
        LogError("Could not open file: " << filename );
        return false;
    }


    std::string fileContent((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

    
    // Check if file is empty
    if (fileContent.size() == 0){
        LogWarn("File is empty: " << filename);
        return false;
    }

    source_ = fileContent;
    
    file.close();

    // convert windows line breaks to unix, since some Python versions
    // seem to have problems with them
    replaceInString(source_,"\r\n","\n");
    
    if (pCompile)
        return compile();

    return true;
}

bool PythonScript::compile(bool logErrors) {
    LogInfo("Compiling script");

    Py_XDECREF(byteCode_);
    byteCode_ = Py_CompileString(source_.c_str(), "", Py_file_input);
    compiled_ = checkCompileError(logErrors);

    if (!compiled_) {
        Py_XDECREF(byteCode_);
        byteCode_ = 0;
    }

    return compiled_;
}

bool PythonScript::run(bool logErrors) {
    PyObject* glb = PyDict_New();
    PyDict_SetItemString(glb, "__builtins__", PyEval_GetBuiltins());

    bool success;
    if (compiled_){
        ivwAssert(byteCode_, "No byte code");
        LogInfo("Running compiled script ...");

        PyObject* ret = PyEval_EvalCode((PyCodeObject*)byteCode_, glb, glb);
        success = checkRuntimeError(logErrors);
        Py_XDECREF(ret);
    }
    else {
        LogInfo("Running script ...");
        PyRun_String(source_.c_str(), Py_file_input, glb, glb);
        success = checkRuntimeError(logErrors);
    }

    Py_XDECREF(glb);

    if (success) {
        LogInfo("finished.");
    }

    return success;
}

std::string PythonScript::getSource() const {
    return source_;
}

void PythonScript::setSource(const std::string& source) {
    source_ = source;
    compiled_ = false;
    Py_XDECREF(byteCode_);
    byteCode_ = 0;
}

bool PythonScript::checkCompileError(bool logErrors) {
    using std::string;

    log_ = "";
    errorLine_ = -1;
    errorCol_ = -1;
    if (!PyErr_Occurred())
        return true;

    PyObject *errtype, *errvalue, *traceback;
    PyErr_Fetch(&errtype, &errvalue, &traceback);

    char* msg = 0;
    PyObject* obj = 0;
    if (PyArg_ParseTuple(errvalue, "sO", &msg, &obj)) {
        int line, col;
        char *code = 0;
        char *mod = 0;
        if (PyArg_ParseTuple(obj, "siis", &mod, &line, &col, &code)) {
            errorLine_ = line;
            errorCol_ = col;
            log_ = "[" + toString(line) + ":" + toString(col) + "] " + toString(msg) + ": " + toString(code);
        }
    }

    // convert error to string, if it could not be parsed
    if (log_.empty()) {
        if (logErrors)
            LogWarn("Failed to parse exception, printing as string:");
        PyObject* s = PyObject_Str(errvalue);
        if (s && PyString_AsString(s)) {
            log_ = std::string(PyString_AsString(s));
            Py_XDECREF(s);
        }
    }

    if (printErrorsToStdOut_) {
        PyErr_Restore(errtype, errvalue, traceback);
        PyErr_Print();
    }
    else {
        Py_XDECREF(errtype);
        Py_XDECREF(errvalue);
        Py_XDECREF(traceback);
    }

    if (logErrors)
        LogError(log_);

    return false;
}

bool PythonScript::checkRuntimeError(bool logErrors) {
    using std::string;

    log_ = "";
    errorLine_ = -1;
    errorCol_ = -1;
    if (!PyErr_Occurred())
        return true;

    std::string pyException = "";
    PyObject* pyError_type = 0;
    PyObject* pyError_value = 0;
    PyObject* pyError_traceback = 0;
    PyObject* pyError_string = 0;
    PyErr_Fetch(&pyError_type, &pyError_value, &pyError_traceback);


    std::string stacktraceStr;
    if (pyError_traceback) {
        PyTracebackObject* traceback = (PyTracebackObject*)pyError_traceback;
        while (traceback) {
            PyFrameObject* frame = traceback->tb_frame;
            std::string stacktraceLine;
            if (frame && frame->f_code) {
                PyCodeObject* codeObject = frame->f_code;
                if (PyString_Check(codeObject->co_filename))
                    stacktraceLine.append(string("  File \"") + PyString_AsString(codeObject->co_filename) + string("\", "));

                errorLine_ = PyCode_Addr2Line(codeObject, frame->f_lasti);
                stacktraceLine.append(string("line ") + toString(errorLine_));

                if (PyString_Check(codeObject->co_name))
                    stacktraceLine.append(string(", in ") + PyString_AsString(codeObject->co_name));
            }
            stacktraceLine.append("\n");
            stacktraceStr = stacktraceLine + stacktraceStr;

            traceback = traceback->tb_next;
        }
    }

    std::stringstream s;
    s << errorLine_;
    pyException.append(string("[") + s.str() + string("] "));
    if (pyError_value && (pyError_string = PyObject_Str(pyError_value)) != 0 && (PyString_Check(pyError_string))) {
        pyException.append(PyString_AsString(pyError_string));
        Py_XDECREF(pyError_string);
        pyError_string = 0;
    }
    else {
        pyException.append("<No data available>");
    }
    pyException.append("\n");

    // finally append stacktrace string
    if (!stacktraceStr.empty()) {
        pyException.append("Stacktrace (most recent call first):\n");
        pyException.append(stacktraceStr);
    }
    else {
        pyException.append("<No stacktrace available>");
        if (logErrors)
            LogWarn("Failed to parse traceback");
    }

    if (printErrorsToStdOut_) {
        PyErr_Restore(pyError_type, pyError_value, pyError_traceback);
        PyErr_Print();
    }
    else {
        Py_XDECREF(pyError_type);
        Py_XDECREF(pyError_value);
        Py_XDECREF(pyError_traceback);
    }

    log_ = pyException;

    if (logErrors)
        LogError(log_);

    return false;
}

} // namespace voreen
