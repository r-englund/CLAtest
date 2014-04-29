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

#include <modules/pypackages/processors/pyprocessorbase.h>
#include <inviwo/core/util/formats.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

PyProcessorBase::PyProcessorBase()
    : Processor()
	 , pythonScriptFile_("pythonScript", "Edit Script", \
		InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"pypackages/scripts/pyavailablepackagesinfo.py", \
		PropertyOwner::INVALID_OUTPUT, PropertySemantics("TextEditor"))	
    , runScript_("runScript", "Run Script")
	, script_()

{
	addProperty(pythonScriptFile_);
    pythonScriptFile_.onChange(this, &PyProcessorBase::loadPythonScriptFile);
    addProperty(runScript_);
    runScript_.onChange(this, &PyProcessorBase::onRunScriptButtonClicked);
}

PyProcessorBase::~PyProcessorBase() {}

void PyProcessorBase::initialize() {
    Processor::initialize();
}

void PyProcessorBase::deinitialize() {
    Processor::deinitialize();
}

void PyProcessorBase::process() {
    Processor::process();
	//derived class process function should not be called after this
	runScript();
}

void PyProcessorBase::onRunScriptButtonClicked() {	
    loadPythonScriptFile();
    //runScript();
}

void PyProcessorBase::loadPythonScriptFile() {
	std::string scriptFileName = pythonScriptFile_.get();
	std::ifstream file(scriptFileName.c_str());
	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	script_.setSource(text);
}

void PyProcessorBase::runScript() {	
	if (URLParser::fileExists(pythonScriptFile_.get())) {
		
		Clock c;
        c.start();
		//TODO: Remove try catch
		try {			
			if (script_.run()) {
				LogInfo("PyCUDAImageInverter script ended")
			}
			c.stop();
		}
		catch(...) {
			c.stop();
			LogError("PyCUDAImageInverter script failed to run");
		}
		LogInfo("Execution time: " << c.getElapsedMiliseconds() << " ms");
	}	 
}

bool PyProcessorBase::allocatePyBuffer(std::string bufferName, std::string bufferType, size_t bufferSize) {
	//allocate and cache buffer
	if (pyBufferMap_.find(bufferName)==pyBufferMap_.end()) {
		Buffer* buffer = 0;
		//buffer = new Buffer(bufferSize, DataFLOAT32::get());
		if (bufferType == DataFLOAT16::str()) 	    buffer = new Buffer_FLOAT16();		
		else if (bufferType == DataFLOAT32::str())	buffer = new Buffer_FLOAT32();
		else if (bufferType == DataFLOAT64::str())	buffer = new Buffer_FLOAT64();
		else if (bufferType == DataINT8::str())   	buffer = new Buffer_INT8();
		//else if (bufferType == DataINT12::str())    buffer = new Buffer_INT12();
		else if (bufferType == DataINT16::str())	buffer = new Buffer_INT16();
		else if (bufferType == DataINT32::str())    buffer = new Buffer_INT32();
		else if (bufferType == DataINT64::str())    buffer = new Buffer_INT64();
		else if (bufferType == DataUINT8::str())    buffer = new Buffer_UINT8();
		//else if (bufferType == DataUINT12::str())   buffer = new Buffer_UINT12();
		else if (bufferType == DataUINT16::str())   buffer = new Buffer_UINT16();
		else if (bufferType == DataUINT32::str())   buffer = new Buffer_UINT32();
		else if (bufferType == DataUINT64::str())   buffer = new Buffer_UINT64();

		if (!buffer) {
			LogWarn("Invalid Buffer Type requested");
			return false;
		}

		pyBufferMap_[bufferName] = buffer;
		buffer->setSize(bufferSize);
		BufferRAM* bufferRAM = buffer->getEditableRepresentation<BufferRAM>();		
		bufferRAM->initialize();
		if (bufferRAM->getSize()==bufferSize)
			return true;
		return true;
	}
	return false;
}

Buffer* PyProcessorBase::getAllocatedPyBuffer(std::string bufferName) {
    //retrieve allocated buffer
    Buffer* buffer = 0;
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end()) {
        buffer = pyBufferMap_[bufferName];        
    }
    return buffer;
}

void* PyProcessorBase::getAllocatedPyBufferData(std::string bufferName) {
    //retrieve allocated buffer
    void* bufferData=0;
    Buffer* buffer = 0;
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end()) {
        buffer = pyBufferMap_[bufferName];   
        bufferData = buffer->getEditableRepresentation<BufferRAM>()->getData();        
    }
    return bufferData;
}

bool PyProcessorBase::isValidPyBufferData(std::string bufferName) {
	//retrieve allocated buffer	
	if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end()) {		
		return true;
	}	
	return false;
}

std::string PyProcessorBase::getPyBufferType(std::string bufferName) {
    //retrieve allocated buffer
    Buffer* buffer = 0;
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end()) {
        buffer = pyBufferMap_[bufferName];
        return buffer->getEditableRepresentation<BufferRAM>()->getDataFormat()->getString();
    }
    return "";
}

std::vector<std::string> PyProcessorBase::getSupportedBufferTypes() {
	std::vector<std::string> bufferTypes;
	bufferTypes.push_back(DataFLOAT16::str());
	bufferTypes.push_back(DataFLOAT32::str());
	bufferTypes.push_back(DataFLOAT64::str());		
	bufferTypes.push_back(DataINT8::str());
	//bufferTypes.push_back(DataINT12::str());
	bufferTypes.push_back(DataINT16::str());
	bufferTypes.push_back(DataINT32::str());
	bufferTypes.push_back(DataINT64::str());		
	bufferTypes.push_back(DataUINT8::str());
	//bufferTypes.push_back(DataUINT12::str());
	bufferTypes.push_back(DataUINT16::str()); 
	bufferTypes.push_back(DataUINT32::str());
	bufferTypes.push_back(DataUINT64::str()); 
	return bufferTypes;
}

void PyProcessorBase::deallocatePyBuffer(std::string bufferName) {
    std::map<std::string, Buffer*>::iterator it = pyBufferMap_.find(bufferName);
    if (it!=pyBufferMap_.end()) {
        Buffer* buffer = it->second;
        pyBufferMap_.erase(it);
        delete buffer;
    }
}

void PyProcessorBase::freeAllBuffers() {
    std::map<std::string, Buffer*>::iterator it;
    for (it=pyBufferMap_.begin(); it!=pyBufferMap_.end(); ++it) {
        delete it->second;
    }
    pyBufferMap_.clear();

}

} // namespace
