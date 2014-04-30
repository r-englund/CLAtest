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

#include <modules/pypackages/processors/pycuda/pycudaimageinvert.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>
#include <modules/pypackages/pypackagesmodule.h>

namespace inviwo {

ProcessorClassName(PyCUDAImageInverter, "PyCUDAImageInverter");
ProcessorCategory(PyCUDAImageInverter, "PyCUDA");
ProcessorCodeState(PyCUDAImageInverter, CODE_STATE_EXPERIMENTAL);

PyCUDAImageInverter::PyCUDAImageInverter()
    : PyProcessorBase()
    , inport_("inport")
	, outport_("outport")
	, cudaKernelFile_("cudaKernel", "Edit Kernel", \
	    InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"pypackages/scripts/pycuda/pycudaimageinvert.cu", \
		PropertyOwner::INVALID_OUTPUT, PropertySemantics("TextEditor"))	
	, invertOptions_("invertOption", "Image Invert Options", PropertyOwner::INVALID_OUTPUT)
{
    addPort(inport_);
	addPort(outport_);
	addProperty(cudaKernelFile_);

    invertOptions_.addOption("nopython", "No Python");
    invertOptions_.addOption("numpy", "NumPy");
    invertOptions_.addOption("pycuda", "PyCUDA");
	addProperty(invertOptions_);

	pythonScriptFile_.set(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+\
						  "pypackages/scripts/pycuda/pycudaimageinvert.py");

    cudaKernelFile_.onChange(this, &PyCUDAImageInverter::loadCUDAKernelFile);
}

PyCUDAImageInverter::~PyCUDAImageInverter() {}

void PyCUDAImageInverter::initialize() {
    PyProcessorBase::initialize();
    allocateBuffers();
}

void PyCUDAImageInverter::deinitialize() {
    PyProcessorBase::deinitialize();
    deAllocateBuffers();
}

void PyCUDAImageInverter::process() {	
    Image* newData = inport_.getData()->clone();
    
	if (invertOptions_.get()=="nopython") {
		cpuInvert(newData);
	    Processor::process();
	}    	
    else if (invertOptions_.get()=="pycuda") {        
        bool pycudaAvailable = InviwoApplication::getPtr()->getModuleByType<PyPackagesModule>()->isPackageAvailable("pycuda");
        //Check if pycuda package is available
        if (pycudaAvailable) {
            PyProcessorBase::process();
        }
        else {
            LogWarn("PyCUDA pacakage not available")
        }
    }
    else if (invertOptions_.get()=="numpy") {       
        PyProcessorBase::process();
    }    

    outport_.setData(newData);
}

void PyCUDAImageInverter::cpuInvert(Image* newImageData) {
	ImageRAM*  outImage = newImageData->getEditableRepresentation<ImageRAM>();	
    uvec2 dim(outImage->getDimension());
	uvec2 pos;
	vec4 intensity;
	LayerRAM* colorLayerRAM = outImage->getColorLayerRAM();
	for(pos.y = 0;pos.y < dim.y;pos.y++) {
		for(pos.x = 0;pos.x < dim.x;pos.x++) {
			//LogWarn(colorLayerRAM->getDataFormatString())
			intensity = colorLayerRAM->getValueAsVec4Float(pos);
			intensity = vec4(fabs(1.0f-intensity.x), fabs(1.0f-intensity.y), fabs(1.0f-intensity.z), intensity.w);
			colorLayerRAM->setValueFromVec4Float(uvec2(pos), intensity*255.0f);
		}
	}
}

void PyCUDAImageInverter::allocateBuffers() {
    loadCUDAKernelFile();
}

void PyCUDAImageInverter::deAllocateBuffers() {
    PyProcessorBase::freeAll();
}

void PyCUDAImageInverter::loadCUDAKernelFile() {
	std::string scriptFileName = cudaKernelFile_.get();
	std::ifstream file(scriptFileName.c_str());
	std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	size_t bufferSize = text.size();

    if (!bufferSize) {
        LogWarn("Invalid CUDA Kernel");
        return;
    }

	//allocate buffer for cuda kernel code (char buffer)
	PyProcessorBase::allocatePyBuffer("ImageInvert_KernelSrc", DataUINT8::str(), bufferSize);
	void* kernelSrc = 0;
	//fetch buffer data
	if (PyProcessorBase::isValidPyBuffer("ImageInvert_KernelSrc")) {
        kernelSrc = PyProcessorBase::getPyBufferData("ImageInvert_KernelSrc");
		if (kernelSrc) {
			char* csrc = static_cast<char*>(kernelSrc);
			//copy to buffer data which is later accessed by python
			std::strncpy(csrc, text.c_str(), bufferSize);
		}
		else
			LogWarn("Invalid buffer data requested")
	}
}

} // namespace
