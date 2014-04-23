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
 * Main file authors: Erik Sundén, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/util/formats.h>

namespace inviwo {

DataFormatBase* DataFormatBase::instance_[] = {NULL};

DataFormatBase::DataFormatBase()
    : formatId_(id())
    , bitsAllocated_(bitsAllocated())
    , bitsStored_(bitsStored())
    , components_(components()) {
    formatStr_ = new std::string(str());
}

DataFormatBase::DataFormatBase(DataFormatEnums::Id t, size_t bA, size_t bS, int c, double max, double min, DataFormatEnums::NumericType nt, std::string s)
    : formatId_(t)
    , bitsAllocated_(bA)
    , bitsStored_(bS)
    , components_(c)
    , numericType_(nt)
    , max_(max)
    , min_(min) {
    formatStr_ = new std::string(s);
}

DataFormatBase::~DataFormatBase() {
    delete formatStr_;
}

const DataFormatBase* DataFormatBase::get(std::string name) {
    if (name == "") return instance_[DataFormatEnums::NOT_SPECIALIZED];

#define DataFormatIdMacro(i) else if(name == #i) return Data##i::get();
#include <inviwo/core/util/formatsdefinefunc.h>
    else if (name == "UCHAR") return DataUINT8::get();
    else if (name == "CHAR") return DataINT8::get();
    else if (name == "USHORT") return DataUINT16::get();
    else if (name == "USHORT_12") return DataUINT12::get();
    else if (name == "SHORT") return DataINT16::get();
    else if (name == "UINT") return DataUINT32::get();
    else if (name == "INT") return DataINT32::get();
    else if (name == "FLOAT") return DataFLOAT32::get();
    else if (name == "DOUBLE") return DataFLOAT64::get();
    else return instance_[DataFormatEnums::NOT_SPECIALIZED];
}

void DataFormatBase::cleanDataFormatBases() {
    for (int i = 0; i<DataFormatEnums::NUMBER_OF_FORMATS; i++) {
        if (instance_[i]) {
            delete instance_[i];
            instance_[i] = 0;
        }
    }
}

float DataFormatBase::valueToNormalizedFloat(void*) const {
    return 0.f;
}

vec2 DataFormatBase::valueToNormalizedVec2Float(void*) const {
    return vec2(0.f);
}

vec3 DataFormatBase::valueToNormalizedVec3Float(void*) const {
    return vec3(0.f);
}

vec4 DataFormatBase::valueToNormalizedVec4Float(void*) const {
    return vec4(0.f);
}

void DataFormatBase::floatToValue(float val, void* loc) const {
    loc = &val;
}

void DataFormatBase::vec2ToValue(vec2 val, void* loc) const {
    loc = &val;
}

void DataFormatBase::vec3ToValue(vec3 val, void* loc) const {
    loc = &val;
}

void DataFormatBase::vec4ToValue(vec4 val, void* loc) const {
    loc = &val;
}

size_t DataFormatBase::getBitsAllocated() const {
    return bitsAllocated_;
}

DataFormatEnums::NumericType DataFormatBase::getNumericType() const {
    return numericType_;
}

size_t DataFormatBase::getBitsStored() const {
    return bitsStored_;
}

size_t DataFormatBase::getBytesAllocated() const {
    return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsAllocated()))));
}

size_t DataFormatBase::getBytesStored() const {
    return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsStored()))));
}

int DataFormatBase::getComponents() const {
    return components_;
}

double DataFormatBase::getMax() const {
    return max_;
}

double DataFormatBase::getMin() const {
    return min_;
}

const char* DataFormatBase::getString() const {
    return formatStr_->c_str();
}

DataFormatEnums::Id DataFormatBase::getId() const {
    return formatId_;
}

} // namespace
