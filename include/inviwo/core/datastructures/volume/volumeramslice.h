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

#ifndef IVW_VOLUMERAMSLICE_H
#define IVW_VOLUMERAMSLICE_H

#include <inviwo/core/datastructures/image/layerramprecision.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>
#include <inviwo/core/datastructures/geometry/geometrytype.h>

namespace inviwo {

class IVW_CORE_API VolumeRAMSlice : public VolumeOperation {
public:
    VolumeRAMSlice(const VolumeRepresentation* in, CoordinatePlane cPlane, unsigned int sliceNum)
        : VolumeOperation(in), cPlane_(cPlane), sliceNum_(sliceNum) {}
    virtual ~VolumeRAMSlice() {}

    template<typename T, size_t B>
    void evaluate();

    static inline LayerRAM* apply(const VolumeRepresentation* in, CoordinatePlane cPlane, unsigned int sliceNum) {
        VolumeRAMSlice sliceOP = VolumeRAMSlice(in, cPlane, sliceNum);
        in->performOperation(&sliceOP);
        return sliceOP.getOutput<LayerRAM>();
    }

private:
    CoordinatePlane cPlane_;
    unsigned int sliceNum_;
};

template<typename T>
class VolumeRAMPrecision;

template<typename T, size_t B>
class VolumeRAMCustomPrecision;

template<typename T, size_t B>
void VolumeRAMSlice::evaluate() {
    const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());

    if (!volume) {
        setOutput(NULL);
        return;
    }

    /*
    uvec3 dataDims = volume->getDimension();
    if(cPlane_ == XY){ //XY Plane
        if (sliceNum_ >= dataDims.z){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.xy(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.xy(),  COLOR_ONLY);

        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        size_t dataSize = dataDims.x*static_cast<size_t>(volume->getDataFormat()->getBytesAllocated());
        size_t initialStartPos = sliceNum_*dataDims.x*dataDims.y;
        size_t offset;
        for (size_t j=0; j < dataDims.y; j++) {
            offset = (j*dataDims.x);
            memcpy(dst + offset, (src + offset + initialStartPos), dataSize);
        }
        setOutput(sliceImage);
    }
    else if(cPlane_ == XZ){ //XZ Plane
        if (sliceNum_ >= dataDims.y){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.xz(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.xz(),  COLOR_ONLY);

        const T* src = reinterpret_cast<const T*>(volume->getData());
        T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        size_t dataSize = dataDims.x*static_cast<size_t>(volume->getDataFormat()->getBytesAllocated());
        size_t initialStartPos = sliceNum_*dataDims.x;
        size_t offsetVolume;
        size_t offsetImage;
        for (size_t j=0; j < dataDims.z; j++) {
            offsetVolume = (j*dataDims.x*dataDims.y) + initialStartPos;
            offsetImage = j*dataDims.x;
            memcpy(dst + offsetImage, src + offsetVolume, dataSize);
        }
        setOutput(sliceImage);
    }
    else{ //YZ Plane
        if (sliceNum_ >= dataDims.x){
            setOutput(NULL);
            return;
        }

        //allocate space
        ImageRAMPrecision<T>* sliceImage;
        if (volume->getDataFormat()->getBitsAllocated() != B)
            sliceImage = new ImageRAMCustomPrecision<T, B>(dataDims.yz(),  COLOR_ONLY);
        else
            sliceImage = new ImageRAMPrecision<T>(dataDims.yz(),  COLOR_ONLY);

        //const T* src = reinterpret_cast<const T*>(volume->getData());
        //T* dst = reinterpret_cast<T*>(sliceImage->getData());

        //copy data
        //size_t offsetVolume;
     //size_t offsetImage;
     // for (size_t i=0; i < dataDims.z; i++) {
     //      for (size_t j=0; j < dataDims.y; j++) {
     //         offsetImage = (i*dataDims.z)+j;
     //         dst[offsetImage] = src[];
     //     }
     // }
        setOutput(sliceImage);
    }*/
}

} // namespace

#endif // IVW_VOLUMERAMSLICE_H
