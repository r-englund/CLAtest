/*
    Have a look at the script.cpp sample and at
    http://docs.python.org/api/api.html
    for more information about binding C/C++ stuff to python
*/

#include "pythonscript.h"

#include <inviwo/core/util/assertion.h>
#include <inviwo/core/util/stringconversion.h>
#include <inviwo/core/util/filedirectory.h>

#include <traceback.h>
#include <frameobject.h>
#include "pythonexecutionoutputobeserver.h"

namespace inviwo {

PythonScript::PythonScript()
    : source_("")
    , byteCode_(0)
    , isCompileNeeded_(false)
{}

PythonScript::~PythonScript() {
    Py_XDECREF(byteCode_);
}

bool PythonScript::compile() {
    LogInfo("Compiling script");

    Py_XDECREF(byteCode_);
    byteCode_ = Py_CompileString(source_.c_str(), "", Py_file_input);
    isCompileNeeded_ = !checkCompileError();

    if (isCompileNeeded_) {
        Py_XDECREF(byteCode_);
        byteCode_ = 0;
    }

    return !isCompileNeeded_;
}

bool PythonScript::run() {
    PyObject* glb = PyDict_New();
    PyDict_SetItemString(glb, "__builtins__", PyEval_GetBuiltins());

    if(isCompileNeeded_ && !compile()){
        LogError("Failed to run script, script could not be compiled");
        return false;
    }

    ivwAssert(byteCode_!=0, "No byte code");
    LogInfo("Running compiled script ...");

    PyObject* ret = PyEval_EvalCode((PyCodeObject*)byteCode_, glb, glb);
    bool success = checkRuntimeError();
    
    Py_XDECREF(ret); 
    Py_XDECREF(glb);

    return success;
}

std::string PythonScript::getSource() const {
    return source_;
}

void PythonScript::setSource(const std::string& source) {
    source_ = source;
    isCompileNeeded_ = true;
    Py_XDECREF(byteCode_);
    byteCode_ = 0;
}

bool PythonScript::checkCompileError() {
    if (!PyErr_Occurred())
        return true;

    PyObject *errtype, *errvalue, *traceback;
    PyErr_Fetch(&errtype, &errvalue, &traceback);


    std::string log = "";
    int errorLine = -1;
    int errorCol = -1;

    char* msg = 0;
    PyObject* obj = 0;
    if (PyArg_ParseTuple(errvalue, "sO", &msg, &obj)) {
        int line, col;
        char *code = 0;
        char *mod = 0;
        if (PyArg_ParseTuple(obj, "siis", &mod, &line, &col, &code)) {
            errorLine = line;
            errorCol = col;
            log = "[" + toString(line) + ":" + toString(col) + "] " + toString(msg) + ": " + toString(code);
        }
    }

    // convert error to string, if it could not be parsed
    if (log.empty()) {
        LogWarn("Failed to parse exception, printing as string:");
        PyObject* s = PyObject_Str(errvalue);
        if (s && PyString_AsString(s)) {
            log = std::string(PyString_AsString(s));
            Py_XDECREF(s);
        }
    }

   
    Py_XDECREF(errtype);
    Py_XDECREF(errvalue);
    Py_XDECREF(traceback);

    LogError(log);

    return false;
}

bool PythonScript::checkRuntimeError() {
    if (!PyErr_Occurred())
        return true;

    std::string pyException = "";
    PyObject* pyError_type = 0;
    PyObject* pyError_value = 0;
    PyObject* pyError_traceback = 0;
    PyObject* pyError_string = 0;
    PyErr_Fetch(&pyError_type, &pyError_value, &pyError_traceback);

    int errorLine = -1;
    int errorCol = -1;


    std::string stacktraceStr;
    if (pyError_traceback) {
        PyTracebackObject* traceback = (PyTracebackObject*)pyError_traceback;
        while (traceback) {
            PyFrameObject* frame = traceback->tb_frame;
            std::string stacktraceLine;
            if (frame && frame->f_code) {
                PyCodeObject* codeObject = frame->f_code;
                if (PyString_Check(codeObject->co_filename))
                    stacktraceLine.append(std::string("  File \"") + PyString_AsString(codeObject->co_filename) + std::string("\", "));

                errorLine = PyCode_Addr2Line(codeObject, frame->f_lasti);
                stacktraceLine.append(std::string("line ") + toString(errorLine));

                if (PyString_Check(codeObject->co_name))
                    stacktraceLine.append(std::string(", in ") + PyString_AsString(codeObject->co_name));
            }
            stacktraceLine.append("\n");
            stacktraceStr = stacktraceLine + stacktraceStr;

            traceback = traceback->tb_next;
        }
    }

    std::stringstream s;
    s << errorLine;
    pyException.append(std::string("[") + s.str() + std::string("] "));
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
        LogWarn("Failed to parse traceback");
    }

    Py_XDECREF(pyError_type);
    Py_XDECREF(pyError_value);
    Py_XDECREF(pyError_traceback);

   LogError(pyException);
   PythonExecutionOutputObeserver::pyhonExecutionOutputEvent(pyException,PythonExecutionOutputObeserver::error);

    return false;
}

} // namespace voreen
