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

#ifndef IVW_VOLUMERAMSUBSAMPLE_H
#define IVW_VOLUMERAMSUBSAMPLE_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>

namespace inviwo {

class IVW_CORE_API VolumeRAMSubSample : public VolumeOperation {
public:
    enum FACTOR{
        HALF=2
    };

    VolumeRAMSubSample(const VolumeRepresentation* in, FACTOR factor) : VolumeOperation(in), factor_(factor) {}
    virtual ~VolumeRAMSubSample() {}

    template<typename T, size_t B>
    void evaluate();

    static inline VolumeRAM* apply(const VolumeRepresentation* in, FACTOR factor) {
        VolumeRAMSubSample subsampleOP = VolumeRAMSubSample(in, factor);
        in->performOperation(&subsampleOP);
        return subsampleOP.getOutput<VolumeRAM>();
    }

private:
    FACTOR factor_;
};

template<typename T>
class VolumeRAMPrecision;

template<typename T, size_t B>
class VolumeRAMCustomPrecision;

template<typename T, size_t B>
void VolumeRAMSubSample::evaluate() {
    const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());

    if (!volume) {
        setOutput(NULL);
        return;
    }

    uvec3 dataDims = volume->getDimension();
    size_t sXY = static_cast<size_t>(dataDims.x*dataDims.y);
    size_t sX = static_cast<size_t>(dataDims.x);

    //calculate new size
    uvec3 newDims = dataDims / static_cast<unsigned int>(factor_);
    size_t dXY = static_cast<size_t>(newDims.x*newDims.y);
    size_t dX = static_cast<size_t>(newDims.x);

    //allocate space
    VolumeRAMPrecision<T>* newVolume = new VolumeRAMCustomPrecision<T, B>(newDims);

    //get data pointers
    const T* src = reinterpret_cast<const T*>(volume->getData());
    T* dst = reinterpret_cast<T*>(newVolume->getData());

    //Half sampling
    if(factor_ == HALF){
        int x,y,z;
        #pragma omp parallel for
        for (z=0; z < static_cast<int>(newDims.z); ++z) {
            for (y=0; y < static_cast<int>(newDims.y); ++y) {
                for (x=0; x < static_cast<int>(newDims.x); ++x) {
                    size_t px = static_cast<size_t>(x*2);
                    size_t py = static_cast<size_t>(y*2);
                    size_t pz = static_cast<size_t>(z*2);
                    T val = src[(pz*sXY) + (py*sX) + px];
                    val += src[(pz*sXY) + (py*sX) + (px+1)];
                    val += src[(pz*sXY) + ((py+1)*sX) + px];
                    val += src[(pz*sXY) + ((py+1)*sX) + (px+1)];
                    val += src[((pz+1)*sXY) + (py*sX) + px];
                    val += src[((pz+1)*sXY) + (py*sX) + (px+1)];
                    val += src[((pz+1)*sXY) + ((py+1)*sX) + px];
                    val += src[((pz+1)*sXY) + ((py+1)*sX) + (px+1)];
                    dst[(z*dXY) + (y*dX) + x] = val*T(0.125);
                }
            }
        }
    }

    setOutput(newVolume);
}

} // namespace

#endif // IVW_VOLUMERAMSUBSAMPLE_H
