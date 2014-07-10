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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_VOLUMEOPERATION_H
#define IVW_VOLUMEOPERATION_H

#include <inviwo/core/datastructures/dataoperation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class VolumeRepresentation;

class IVW_CORE_API VolumeOperation : public DataOperation {
public:
    VolumeOperation(const VolumeRepresentation* in) : DataOperation(), in_(in) {}
    virtual ~VolumeOperation() {}

    const VolumeRepresentation* getInputVolume() { return in_; }


    template<typename VO>
    void evaluateFor(){
        VO* t = dynamic_cast<VO*>(this);
        if(!t) return; //todo maybe print error= 
        switch (getInputVolume()->getDataFormat()->getId()) {
        case DataFormatEnums::FLOAT16: t->evaluate<DataFLOAT16::type, DataFLOAT16::bits>(); break; 
        case DataFormatEnums::FLOAT32: t->evaluate<DataFLOAT32::type, DataFLOAT32::bits>(); break; 
        case DataFormatEnums::FLOAT64: t->evaluate<DataFLOAT64::type, DataFLOAT64::bits>(); break; 
        case DataFormatEnums::INT8: t->evaluate<DataINT8::type, DataINT8::bits>(); break; 
        case DataFormatEnums::INT12: t->evaluate<DataINT12::type, DataINT12::bits>(); break; 
        case DataFormatEnums::INT16: t->evaluate<DataINT16::type, DataINT16::bits>(); break; 
        case DataFormatEnums::INT32: t->evaluate<DataINT32::type, DataINT32::bits>(); break; 
        case DataFormatEnums::INT64: t->evaluate<DataINT64::type, DataINT64::bits>(); break; 
        case DataFormatEnums::UINT8: t->evaluate<DataUINT8::type, DataUINT8::bits>(); break; 
        case DataFormatEnums::UINT12: t->evaluate<DataUINT12::type, DataUINT12::bits>(); break; 
        case DataFormatEnums::UINT16: t->evaluate<DataUINT16::type, DataUINT16::bits>(); break; 
        case DataFormatEnums::UINT32: t->evaluate<DataUINT32::type, DataUINT32::bits>(); break; 
        case DataFormatEnums::UINT64: t->evaluate<DataUINT64::type, DataUINT64::bits>(); break; 
        case DataFormatEnums::Vec2FLOAT16: t->evaluate<DataVec2FLOAT16::type, DataVec2FLOAT16::bits>(); break; 
        case DataFormatEnums::Vec2FLOAT32: t->evaluate<DataVec2FLOAT32::type, DataVec2FLOAT32::bits>(); break; 
        case DataFormatEnums::Vec2FLOAT64: t->evaluate<DataVec2FLOAT64::type, DataVec2FLOAT64::bits>(); break; 
        case DataFormatEnums::Vec2INT8: t->evaluate<DataVec2INT8::type, DataVec2INT8::bits>(); break; 
        case DataFormatEnums::Vec2INT12: t->evaluate<DataVec2INT12::type, DataVec2INT12::bits>(); break; 
        case DataFormatEnums::Vec2INT16: t->evaluate<DataVec2INT16::type, DataVec2INT16::bits>(); break; 
        case DataFormatEnums::Vec2INT32: t->evaluate<DataVec2INT32::type, DataVec2INT32::bits>(); break; 
        case DataFormatEnums::Vec2INT64: t->evaluate<DataVec2INT64::type, DataVec2INT64::bits>(); break; 
        case DataFormatEnums::Vec2UINT8: t->evaluate<DataVec2UINT8::type, DataVec2UINT8::bits>(); break; 
        case DataFormatEnums::Vec2UINT12: t->evaluate<DataVec2UINT12::type, DataVec2UINT12::bits>(); break; 
        case DataFormatEnums::Vec2UINT16: t->evaluate<DataVec2UINT16::type, DataVec2UINT16::bits>(); break; 
        case DataFormatEnums::Vec2UINT32: t->evaluate<DataVec2UINT32::type, DataVec2UINT32::bits>(); break; 
        case DataFormatEnums::Vec2UINT64: t->evaluate<DataVec2UINT64::type, DataVec2UINT64::bits>(); break;
        case DataFormatEnums::Vec3FLOAT16: t->evaluate<DataVec3FLOAT16::type, DataVec3FLOAT16::bits>(); break; 
        case DataFormatEnums::Vec3FLOAT32: t->evaluate<DataVec3FLOAT32::type, DataVec3FLOAT32::bits>(); break; 
        case DataFormatEnums::Vec3FLOAT64: t->evaluate<DataVec3FLOAT64::type, DataVec3FLOAT64::bits>(); break; 
        case DataFormatEnums::Vec3INT8: t->evaluate<DataVec3INT8::type, DataVec3INT8::bits>(); break; 
        case DataFormatEnums::Vec3INT12: t->evaluate<DataVec3INT12::type, DataVec3INT12::bits>(); break; 
        case DataFormatEnums::Vec3INT16: t->evaluate<DataVec3INT16::type, DataVec3INT16::bits>(); break; 
        case DataFormatEnums::Vec3INT32: t->evaluate<DataVec3INT32::type, DataVec3INT32::bits>(); break; 
        case DataFormatEnums::Vec3INT64: t->evaluate<DataVec3INT64::type, DataVec3INT64::bits>(); break; 
        case DataFormatEnums::Vec3UINT8: t->evaluate<DataVec3UINT8::type, DataVec3UINT8::bits>(); break; 
        case DataFormatEnums::Vec3UINT12: t->evaluate<DataVec3UINT12::type, DataVec3UINT12::bits>(); break; 
        case DataFormatEnums::Vec3UINT16: t->evaluate<DataVec3UINT16::type, DataVec3UINT16::bits>(); break; 
        case DataFormatEnums::Vec3UINT32: t->evaluate<DataVec3UINT32::type, DataVec3UINT32::bits>(); break; 
        case DataFormatEnums::Vec3UINT64: t->evaluate<DataVec3UINT64::type, DataVec3UINT64::bits>(); break; 
        case DataFormatEnums::Vec4FLOAT16: t->evaluate<DataVec4FLOAT16::type, DataVec4FLOAT16::bits>(); break; 
        case DataFormatEnums::Vec4FLOAT32: t->evaluate<DataVec4FLOAT32::type, DataVec4FLOAT32::bits>(); break; 
        case DataFormatEnums::Vec4FLOAT64: t->evaluate<DataVec4FLOAT64::type, DataVec4FLOAT64::bits>(); break; 
        case DataFormatEnums::Vec4INT8: t->evaluate<DataVec4INT8::type, DataVec4INT8::bits>(); break; 
        case DataFormatEnums::Vec4INT12: t->evaluate<DataVec4INT12::type, DataVec4INT12::bits>(); break; 
        case DataFormatEnums::Vec4INT16: t->evaluate<DataVec4INT16::type, DataVec4INT16::bits>(); break; 
        case DataFormatEnums::Vec4INT32: t->evaluate<DataVec4INT32::type, DataVec4INT32::bits>(); break; 
        case DataFormatEnums::Vec4INT64: t->evaluate<DataVec4INT64::type, DataVec4INT64::bits>(); break; 
        case DataFormatEnums::Vec4UINT8: t->evaluate<DataVec4UINT8::type, DataVec4UINT8::bits>(); break; 
        case DataFormatEnums::Vec4UINT12: t->evaluate<DataVec4UINT12::type, DataVec4UINT12::bits>(); break; 
        case DataFormatEnums::Vec4UINT16: t->evaluate<DataVec4UINT16::type, DataVec4UINT16::bits>(); break; 
        case DataFormatEnums::Vec4UINT32: t->evaluate<DataVec4UINT32::type, DataVec4UINT32::bits>(); break; 
        case DataFormatEnums::Vec4UINT64: t->evaluate<DataVec4UINT64::type, DataVec4UINT64::bits>(); break; 
        default: break; 
        }
    }

private:
    const VolumeRepresentation* in_;
};

} // namespace

#endif // IVW_VOLUMEOPERATION_H
