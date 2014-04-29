/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/pypackagesinterface/pypackagesutil.h>
#include <modules/python/pythoninterface/pyvalueparser.h>
#include <modules/pypackages/pypackagesinterface/pypackageparsers.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/pypackages/processors/pyprocessorbase.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL PYPACKAGE_ARRAY_API
#define NO_IMPORT_ARRAY
#include <arrayobject.h>

namespace inviwo {

//////////////////////////////////////////////////////////////////////////
//Buffers

PyObject* py_declareBufferData(PyObject* /*self*/, PyObject* args) {
    static PyDeclareBufferData p;

    if (!p.testParams(args))
        return 0;

    const char* pyProcessorId = 0;
    const char* varName = 0;
	const char* varType = 0;
	const int* varSize = 0;
    
    if (!PyArg_ParseTuple(args, "ssss:declareBufferData", &pyProcessorId, &varName, &varType, &varSize))
        return 0;

    if (pyProcessorId && varName && varType && varSize) {
		if (InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()) {
			std::vector<Processor*> processors  = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

			for (std::vector<Processor*>::const_iterator processorIt = processors.begin(); processorIt!=processors.end(); ++processorIt) {
				if (pyProcessorId == (*processorIt)->getIdentifier()) {
					//check type
					PyProcessorBase* pyProcessor = dynamic_cast<PyProcessorBase*>(*processorIt);					
					if (pyProcessor) {
						std::string bufferName(varName);
						std::string bufferType(varType);
						size_t bufferSize = (size_t) *varSize;
						if (pyProcessor->allocatePyBuffer(bufferName, bufferType, bufferSize))
							std::cout << "Allocating Buffer data : " <<  bufferName \
									  << " of type " << bufferType << " with size " << bufferSize << std::endl;
						else
							std::cout << "Buffer allocation failed" << std::endl;
					}
				}
			}
		}
    }

    Py_RETURN_NONE;
}

PyObject* py_getBufferData(PyObject* /*self*/, PyObject* args) {
    static PyGetBufferData p;

    if (!p.testParams(args))
        return 0;

	const char* pyProcessorId = 0;
    const char* bufferDataName = 0;

    if (!PyArg_ParseTuple(args, "ss:getBufferData", &pyProcessorId, &bufferDataName))
        return 0;

    if (pyProcessorId && bufferDataName) {        
		if (InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()) {
			std::vector<Processor*> processors  = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

			for (std::vector<Processor*>::const_iterator processor = processors.begin(); processor!=processors.end(); ++processor) {
				if (pyProcessorId == (*processor)->getIdentifier()) {
					//check type
					PyProcessorBase* pyProcessor = dynamic_cast<PyProcessorBase*>(*processor);					
					if (pyProcessor) {
						std::string bufferName(bufferDataName);
                        Buffer* rawBuffer = 0;
						void* bufferData = 0;
                        std::string bufferTypeStr("");
                        size_t bufferSize = 0;
						if (pyProcessor->isValidPyBuffer(bufferName)) {
                            rawBuffer = pyProcessor->getAllocatedPyBuffer(bufferName);
                            bufferData = pyProcessor->getAllocatedPyBufferData(bufferName);
                            bufferTypeStr = pyProcessor->getPyBufferType(bufferName);
                            bufferSize = rawBuffer->getSize();
							std::cout << "Getting Buffer data : " <<  bufferName << " of type " << bufferTypeStr << " " \
							          << "-" << " with size " << bufferSize << std::endl;
                        }
						else {
							std::cout << "Buffer fetch failed" << std::endl;
                        }

						if (!bufferData)  Py_RETURN_NONE;
                        if (rawBuffer->getDataFormat()->getComponents()==1) {
                            if (bufferTypeStr == DataFLOAT16::str()) return PyPackageParser::toPyObject<DataFLOAT16>(bufferData, bufferSize);
                            if (bufferTypeStr == DataFLOAT32::str()) return PyPackageParser::toPyObject<DataFLOAT32>(bufferData, bufferSize);
                            if (bufferTypeStr == DataFLOAT64::str()) return PyPackageParser::toPyObject<DataFLOAT64>(bufferData, bufferSize);
                            if (bufferTypeStr == DataINT8::str()) return PyPackageParser::toPyObject<DataINT8>(bufferData, bufferSize);
                            if (bufferTypeStr == DataINT16::str()) return PyPackageParser::toPyObject<DataINT16>(bufferData, bufferSize);
                            if (bufferTypeStr == DataINT32::str()) return PyPackageParser::toPyObject<DataINT32>(bufferData, bufferSize);
                            if (bufferTypeStr == DataINT64::str()) return PyPackageParser::toPyObject<DataINT64>(bufferData, bufferSize);
                            if (bufferTypeStr == DataUINT8::str()) return PyPackageParser::toPyObject<DataUINT8>(bufferData, bufferSize);
                            if (bufferTypeStr == DataUINT16::str()) return PyPackageParser::toPyObject<DataUINT16>(bufferData, bufferSize);
                            if (bufferTypeStr == DataUINT32::str()) return PyPackageParser::toPyObject<DataUINT32>(bufferData, bufferSize);
                            if (bufferTypeStr == DataUINT64::str()) return PyPackageParser::toPyObject<DataUINT64>(bufferData, bufferSize);
                        }
					}
				}
			}
		}         
    }

    Py_RETURN_NONE;
}

PyDeclareBufferData::PyDeclareBufferData()
    : pyprocessorId_("pyprocessorid")
    , bufferVariableName_("buffervariablename")
	, bufferVariableType_("buffervariabletype")
	, bufferSize_("buffervariablesize",1)
{
    addParam(&pyprocessorId_);
    addParam(&bufferVariableName_);
	addParam(&bufferVariableType_);
	addParam(&bufferSize_);
}

PyGetBufferData::PyGetBufferData()
    : pyprocessorId_("pyprocessorid")
    , bufferVariableName_("buffervariablename")
{
    addParam(&pyprocessorId_);
    addParam(&bufferVariableName_);
}

}
