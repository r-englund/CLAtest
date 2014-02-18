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

#ifndef IVW_VOLUMERAMHISTOGRAM_H
#define IVW_VOLUMERAMHISTOGRAM_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>
#include <inviwo/core/datastructures/histogram.h>

namespace inviwo {

class IVW_CORE_API VolumeRAMNormalizedHistogram : public VolumeOperation {
public:
    VolumeRAMNormalizedHistogram(const VolumeRepresentation* in, NormalizedHistogram* oldHist=NULL, int delta=1, size_t maxNumberOfBins = 2048)
        : VolumeOperation(in)
        , delta_(delta)
        , histogramContainer_(oldHist)
        , maxNumberOfBins_(maxNumberOfBins) {
    }

    virtual ~VolumeRAMNormalizedHistogram() {}

    template<typename T, size_t B>
    void evaluate();

    static inline NormalizedHistogram* apply(const VolumeRepresentation* in, NormalizedHistogram* oldHist=NULL, int delta=1) {
        VolumeRAMNormalizedHistogram subsetOP = VolumeRAMNormalizedHistogram(in, oldHist, delta);
        in->performOperation(&subsetOP);
        return subsetOP.getOutput<NormalizedHistogram>();
    }

private:
    int delta_;
    NormalizedHistogram* histogramContainer_;
    size_t maxNumberOfBins_;
};

template<typename T>
class VolumeRAMPrecision;

template<typename T, size_t B>
class VolumeRAMCustomPrecision;

template<typename T, size_t B>
void VolumeRAMNormalizedHistogram::evaluate() {
    const VolumeRAMPrecision<T>* volume = dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());

    if (!volume) {
        setOutput(NULL);
        return;
    }

    const DataFormat<T,B>* dataFormat = dynamic_cast<const DataFormat<T,B>*>(volume->getDataFormat());

    if (!dataFormat) {
        setOutput(NULL);
        return;
    }

    //Calculate number of bins in data
    double numberOfBinsInData = std::pow(2.0, static_cast<double>(dataFormat->getBitsStored()));
    size_t numberOfBinsInHistogram;

    if (numberOfBinsInData > static_cast<double>(maxNumberOfBins_)) {
        numberOfBinsInHistogram = maxNumberOfBins_;
    } else {
        numberOfBinsInHistogram = static_cast<size_t>(numberOfBinsInData);
    }

    //Create Normalized Histogram
    NormalizedHistogram* hist = histogramContainer_;

    if (!hist)
        hist = new NormalizedHistogram(numberOfBinsInHistogram);
    else
        hist->resize(numberOfBinsInHistogram);

    //Calculate histogram data
    std::vector<float>* histData = hist->getData();
    T* src = const_cast<T*>(reinterpret_cast<const T*>(volume->getData()));
    const uvec3& dim = volume->getDimension();
    unsigned int dimXY = dim.x*dim.y;
    size_t idx;
    float intensity;

    for (size_t z=0; z<dim.z-delta_; z+=delta_) {
        for (size_t y=0; y<dim.y-delta_; y+=delta_) {
            for (size_t x=0; x<dim.x-delta_; x+=delta_) {
                intensity = dataFormat->valueToNormalizedFloat(&src[x+(y*dim.x)+(z*dimXY)]);
                // Temporary fix for intensity values outside of 0,1. needed for for float types where
                // the values are not normalized.
                intensity = std::min(std::max(0.0f, intensity), 1.0f);
                idx = static_cast<size_t>(intensity*(numberOfBinsInHistogram-1));
                histData->at(idx)++;
            }
        }
    }

    //Perform normalization
    hist->performNormalization();
    //Set as valid
    hist->setValid(true);
    setOutput(hist);
}

} // namespace

#endif // IVW_VOLUMERAMHISTOGRAM_H
