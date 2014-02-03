 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Rickard Englund
 *
 *********************************************************************************/

#include "pythonqtmethods.h"
#include <modules/python/pythoninterface/pyvalueparser.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/urlparser.h>
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
    if (!PyValueParser::is<std::string>(PyTuple_GetItem(args, 0))) {
        PyErr_SetString(PyExc_TypeError, "loadWorkspace() first argument must be a string");
        return 0;
    }

    std::string filename = PyValueParser::parse<std::string>(PyTuple_GetItem(args, 0));

    if(!URLParser::fileExists(filename)){
        filename = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) + filename;
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
