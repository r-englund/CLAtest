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

#include <modules/pypackages/processors/numpy/numpybuffertest.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/datastructures/image/imageramprecision.h>

namespace inviwo {

ProcessorClassIdentifier(NumpyBufferTest, "org.inviwo.NumpyBufferTest");
ProcessorDisplayName(NumpyBufferTest,  "Numpy Buffer Test");
ProcessorTags(NumpyBufferTest, "Python/CPU");
ProcessorCategory(NumpyBufferTest, "Numpy");
ProcessorCodeState(NumpyBufferTest, CODE_STATE_EXPERIMENTAL);

NumpyBufferTest::NumpyBufferTest()
    : PyProcessorBase() {   
     pythonScriptFile_.set(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+\
            "pypackages/scripts/numpy/numpybuffertest.py");
}

NumpyBufferTest::~NumpyBufferTest() {}

void NumpyBufferTest::initialize() {
    PyProcessorBase::initialize();
    allocateBuffers();
}

void NumpyBufferTest::deinitialize() {
    deAllocateBuffers();
    PyProcessorBase::deinitialize();
}

void NumpyBufferTest::process() {
    PyProcessorBase::process();	
}

void NumpyBufferTest::allocateBuffers() {
    allocatePyBuffer("integerBuffer", DataINT32::str(), 10);
    glm::int32* intBufferData = static_cast<glm::int32*>(getPyBufferData("integerBuffer"));
    for (size_t i=0; i<10; i++) intBufferData[i] = 1;

    allocatePyBuffer("unsignedIntegerBuffer", DataUINT32::str(), 10);
    glm::uint32* uintBufferData = static_cast<glm::uint32*>(getPyBufferData("unsignedIntegerBuffer"));
    for (size_t i=0; i<10; i++) uintBufferData[i] = 1;

    allocatePyBuffer("floatBuffer", DataFLOAT32::str(), 10);
    glm::float32* floatBufferData = static_cast<glm::float32*>(getPyBufferData("floatBuffer"));
    for (size_t i=0; i<10; i++) floatBufferData[i] = 1.0f;
}

void NumpyBufferTest::deAllocateBuffers() {
    PyProcessorBase::freeAll();
}


} // namespace
