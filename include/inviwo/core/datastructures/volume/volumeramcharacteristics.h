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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_VOLUMERAMCHARACTERISTICS_H
#define IVW_VOLUMERAMCHARACTERISTICS_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>

namespace inviwo {

class IVW_CORE_API VolumeRAMCharacteristics : public VolumeOperation {
public:
    struct Characteristics {
        Characteristics() : isUniform(false), mean(0.f) {}
        bool isUniform;
        vec4 mean;
    };

    VolumeRAMCharacteristics(const VolumeRepresentation* in)
        : VolumeOperation(in), stats_(Characteristics()) {}
    virtual ~VolumeRAMCharacteristics() {}

    Characteristics getCharacteristics() const { return stats_; }

    template<typename T, size_t B>
    void evaluate();

    static inline Characteristics apply(const VolumeRepresentation* in) {
        VolumeRAMCharacteristics statOP = VolumeRAMCharacteristics(in);
        in->performOperation(&statOP);
        return statOP.getCharacteristics();
    }
private:
    Characteristics stats_;
};

template<typename T>
class VolumeRAMPrecision;

template<typename T, size_t B>
class VolumeRAMCustomPrecision;

template<typename T, size_t B>
void VolumeRAMCharacteristics::evaluate() {
    const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());

    if (!volume) {
        return;
    }

    const T* src = reinterpret_cast<const T*>(volume->getData());

    glm::i64 numVoxels = static_cast<glm::i64>(volume->getDimension().x*volume->getDimension().y*volume->getDimension().z);
    float weight = static_cast<float>(1/numVoxels);
    const DataFormatBase* format = volume->getDataFormat();

    stats_.isUniform = true;
    T firstVal = src[0];
    stats_.mean = format->valueToVec4Float(&firstVal)*weight;

    #pragma omp for
    for (glm::i64 i=1; i < numVoxels; ++i) {
        T val = src[i];
        stats_.isUniform = (stats_.isUniform ? firstVal != val : stats_.isUniform);
        stats_.mean += format->valueToVec4Float(&val)*weight;
    }
}

} // namespace

#endif // IVW_VOLUMERAMCHARACTERISTICS_H
