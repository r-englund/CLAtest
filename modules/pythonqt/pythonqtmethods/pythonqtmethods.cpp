#include "pythonqtmethods.h"
#include <modules/python/pythoninterface/pyvalueparser.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/qt/editor/networkeditor.h>

namespace inviwo{

PyObject* py_loadWorkspace(PyObject* /*self*/, PyObject* args){

    if (!( PyTuple_Size(args) == 1)) {
        std::ostringstream errStr;
        errStr << "loadWorkspace() takes 1 argument: filename";
        errStr << " (" << PyTuple_Size(args) << " given)";
        PyErr_SetString(PyExc_TypeError, errStr.str().c_str());
        return 0;
    }

    // check parameter if is string
    if (!PyString_Check(PyTuple_GetItem(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "loadWorkspace() first argument must be a string");
        return 0;
    }

    PyValueParser parser;
    std::string filename = parser.parse<std::string>(PyTuple_GetItem(args, 0));

    if(!URLParser::fileExists(filename)){
        filename = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_PROJECT) + filename;
        if(!URLParser::fileExists(filename)){
            std::string msg = std::string("loadWorkspace() could not find file") + filename;
            PyErr_SetString(PyExc_TypeError, msg.c_str());
            return 0;
        }
    }

    NetworkEditor::getPtr()->loadNetwork(filename);

    Py_RETURN_NONE;

}

}
