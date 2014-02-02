/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_VOLUMERAMHISTOGRAM_H
#define IVW_VOLUMERAMHISTOGRAM_H

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeoperation.h>
#include <inviwo/core/datastructures/histogram.h>

namespace inviwo {

class IVW_CORE_API VolumeRAMNormalizedHistogram : public VolumeOperation {
public:
    VolumeRAMNormalizedHistogram(const VolumeRepresentation* in, NormalizedHistogram* oldHist=NULL, int delta=1)
        : VolumeOperation(in)
        , delta_(delta)
        , histogramContainer_(oldHist) {
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
    size_t numberOfBinsInData = static_cast<size_t>(std::pow(2.f, static_cast<float>(dataFormat->getBitsStored())));
    //Histogram will have a maximum of 2^12 bins
    size_t numberOfBinsInHistogram = glm::min<size_t>(4096, numberOfBinsInData);
    //Calculate scaling
    //float binScaling = static_cast<float>(numberOfBinsInHistogram)/static_cast<float>(numberOfBinsInData);
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
