/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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

#ifndef IVW_PYPROCESSORBASE_H
#define IVW_PYPROCESSORBASE_H

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/processors/processor.h>
#include <modules/python/pythonscript.h>
#include <inviwo/core/datastructures/buffer/buffer.h>

namespace inviwo {

class IVW_MODULE_PYPACKAGES_API PyProcessorBase : public Processor {
public:
    PyProcessorBase();
    virtual ~PyProcessorBase();

    virtual void initialize();
    virtual void deinitialize();
    virtual void process();
	bool allocatePyBuffer(std::string bufferName, std::string bufferType, size_t bufferSize);    
	bool isValidPyBufferData(std::string bufferName);
    void* getAllocatedPyBufferData(std::string bufferName);
    Buffer* getAllocatedPyBuffer(std::string bufferName);
    std::string getPyBufferType(std::string bufferName);
	std::vector<std::string> getSupportedBufferTypes();
    void deallocatePyBuffer(std::string bufferName);
    void freeAllBuffers();
	void runScript();
    void onRunScriptButtonClicked();
protected:
	virtual void loadPythonScriptFile();
	FileProperty pythonScriptFile_;
    ButtonProperty runScript_;
private:
	PythonScript script_;
	std::map<std::string, Buffer*> pyBufferMap_;
};

} // namespace

#endif // IVW_PYPROCESSORBASE_H