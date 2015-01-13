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

#ifndef IVW_ELEMENT_BUFFERGL_CONVERTER_H
#define IVW_ELEMENT_BUFFERGL_CONVERTER_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/buffer/elementbuffergl.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ElementBufferRAM2GLConverter : public RepresentationConverterType<ElementBufferGL> {

public:
    ElementBufferRAM2GLConverter();
    virtual ~ElementBufferRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const BufferRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

class IVW_MODULE_OPENGL_API ElementBufferGL2RAMConverter : public RepresentationConverterType<BufferRAM> {

public:
    ElementBufferGL2RAMConverter();
    virtual ~ElementBufferGL2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const ElementBufferGL*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace

#endif // IVW_ELEMENT_BUFFERGL_CONVERTER_H
