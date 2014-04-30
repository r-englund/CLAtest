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
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>


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

			for (std::vector<Processor*>::const_iterator processorIt = processors.begin(); processorIt!=processors.end(); ++processorIt) {
				if (pyProcessorId == (*processorIt)->getIdentifier()) {
					//check type
					PyProcessorBase* pyProcessor = dynamic_cast<PyProcessorBase*>(*processorIt);					
					if (pyProcessor) {
						std::string bufferName(bufferDataName);
                        Buffer* rawBuffer = 0;
						void* bufferData = 0;
                        std::string bufferTypeStr("");
                        size_t bufferSize = 0;
						if (pyProcessor->isValidPyBuffer(bufferName)) {
                            rawBuffer = pyProcessor->getAllocatedPyBuffer(bufferName);
                            bufferData = pyProcessor->getPyBufferData(bufferName);
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

                            #define RETURN_PYOBJECT(i) \
                            case DataFormatEnums::##i: return PyPackageParser::toPyObject<Data##i>(bufferData, bufferSize);
                            #include <modules/pypackages/pypackagesformatsmacro.h>

                            switch (rawBuffer->getDataFormat()->getId()) {
                                PYPACKAGES_FORMAT_MACRO_EXPANDER(RETURN_PYOBJECT)
                                default: break;
                            }
                        }
					}
				}
			}
		}         
    }

    Py_RETURN_NONE;
}

PyObject* py_getLayerData(PyObject* /*self*/, PyObject* args) {
    static PyGetLayerData p;

    if (!p.testParams(args))
        return 0;

    const char* pyProcessorId = 0;
    const char* layerDataName = 0;

    if (!PyArg_ParseTuple(args, "ss:getLayerData", &pyProcessorId, &layerDataName))
        return 0;

    if (pyProcessorId && layerDataName) {        
        if (InviwoApplication::getPtr() && InviwoApplication::getPtr()->getProcessorNetwork()) {
            std::vector<Processor*> processors  = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessors();

            for (std::vector<Processor*>::const_iterator processorIt = processors.begin(); processorIt!=processors.end(); ++processorIt) {
                if (pyProcessorId == (*processorIt)->getIdentifier()) {
                    //check type
                    PyProcessorBase* pyProcessor = dynamic_cast<PyProcessorBase*>(*processorIt);					
                    if (pyProcessor) {
                        std::string layerName(layerDataName);
                        Layer* rawLayer = 0;
                        void* layerData = 0;
                        std::string layerTypeStr("");
                        ivec2 layerDim;
                        if (pyProcessor->isValidPyBuffer(layerName)) {
                            rawLayer = pyProcessor->getAllocatedLayer(layerName);
                            layerData = pyProcessor->getLayerData(layerName);
                            layerTypeStr = pyProcessor->getLayerType(layerName);
                            layerDim = rawLayer->getDimension();
                            std::cout << "Getting Layer data : " <<  layerName << " of type " << layerTypeStr << " " \
                                << "-" << " with dim " << layerDim.x << " " << layerDim.y << std::endl;
                        }
                        else {
                            std::cout << "Layer fetch failed" << std::endl;
                        }

                        if (!layerData)  Py_RETURN_NONE;
                        if (rawLayer->getDataFormat()->getComponents()==2) {
                            #define RETURN_PYOBJECT(i) \
                            case DataFormatEnums::##i: return PyPackageParser::toPyObject<Data##i>(layerData, layerDim);
                            #include <modules/pypackages/pypackagesformatsmacro.h>

                            switch (rawLayer->getDataFormat()->getId()) {
                                PYPACKAGES_FORMAT_MACRO_EXPANDER(RETURN_PYOBJECT)
                                default: break;
                            }
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

PyGetLayerData::PyGetLayerData()
    : pyprocessorId_("pyprocessorid")
    , layerVariableName_("layervariablename")
{
    addParam(&pyprocessorId_);
    addParam(&layerVariableName_);
}

}
