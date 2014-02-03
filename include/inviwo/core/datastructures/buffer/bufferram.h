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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_BUFFER_RAM_H
#define IVW_BUFFER_RAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/buffer/bufferrepresentation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API BufferRAM : public BufferRepresentation {

public:
    BufferRAM(size_t size, const DataFormatBase* format = DataFormatBase::get(), BufferType type = POSITION_ATTRIB, BufferUsage usage = STATIC);
    BufferRAM(const BufferRAM& rhs);
    BufferRAM& operator=(const BufferRAM& that);
    virtual BufferRAM* clone() const = 0;
    virtual ~BufferRAM();

    virtual void initialize();
    virtual void deinitialize();

    virtual std::string getClassName() const { return "BufferRAM"; }

    virtual void setSize(size_t size);
    virtual void resize(size_t size);

    virtual void* getData() = 0;
    virtual const void* getData() const = 0;

    virtual void setValueFromSingleFloat(size_t index, float val) = 0;
    virtual void setValueFromVec2Float(size_t index, vec2 val) = 0;
    virtual void setValueFromVec3Float(size_t index, vec3 val) = 0;
    virtual void setValueFromVec4Float(size_t index, vec4 val) = 0;

    virtual float getValueAsSingleFloat(size_t index) const = 0;
    virtual vec2 getValueAsVec2Float(size_t index) const = 0;
    virtual vec3 getValueAsVec3Float(size_t index) const = 0;
    virtual vec4 getValueAsVec4Float(size_t index) const = 0;
};

/**
 * Factory for buffers.
 * Creates a BufferRAM with data type specified by format.
 *
 * @param size of buffer to create.
 * @param format of buffer to create.
 * @return NULL if no valid format was specified.
 */
IVW_CORE_API BufferRAM* createBufferRAM(size_t size, const DataFormatBase* format, BufferType type, BufferUsage usage);

} // namespace

#endif // IVW_BUFFER_RAM_H
