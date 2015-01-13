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
 *********************************************************************************/

#ifndef IVW_PYCUDAIMAGEINVERT_H
#define IVW_PYCUDAIMAGEINVERT_H

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>
#include <modules/pypackages/pypackagesinterface/pypackagesutil.h>
#include <modules/pypackages/processors/pyprocessorbase.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/processors/processor.h>
#include <modules/python/pythonscript.h>

namespace inviwo {

class IVW_MODULE_PYPACKAGES_API PyCUDAImageInverter : public PyProcessorBase {
public:
    PyCUDAImageInverter();
    ~PyCUDAImageInverter();

    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();
    virtual void process();  
    void onInportChange();

private:    
    void loadCUDAKernelFile();
    void allocateBuffers();
    void deAllocateBuffers();

protected:
    ImageInport inport_;
    ImageOutport outport_;
    FileProperty cudaKernelFile_;   
    Image* cachedImageData_;
};


} // namespace

#endif // IVW_PYCUDAIMAGEINVERT_H