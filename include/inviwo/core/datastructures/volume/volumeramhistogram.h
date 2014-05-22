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
    VolumeRAMNormalizedHistogram(const VolumeRepresentation* in,
                                 std::vector<NormalizedHistogram*>* oldHist = NULL,
                                 int sampleRate = 1, size_t maxNumberOfBins = 2048)
        : VolumeOperation(in)
        , sampleRate_(sampleRate)
        , histograms_(oldHist)
        , maxNumberOfBins_(maxNumberOfBins) {}

    virtual ~VolumeRAMNormalizedHistogram() {}

    template <typename T, size_t B>
    void evaluate();

    static inline std::vector<NormalizedHistogram*>* apply(
        const VolumeRepresentation* in, std::vector<NormalizedHistogram*>* oldHist = NULL,
        int sampleRate = 1, size_t maxNumberOfBins = 2048) {
        VolumeRAMNormalizedHistogram subsetOP =
            VolumeRAMNormalizedHistogram(in, oldHist, sampleRate, maxNumberOfBins);
        in->performOperation(&subsetOP);
        return subsetOP.getOutput<std::vector<NormalizedHistogram*> >();
    }

    static size_t VolumeRAMNormalizedHistogram::calculateBins(const VolumeRAM* volumeRAM,
                                                       size_t maxNumberOfBins) {
        const Volume* volume = reinterpret_cast<const Volume*>(volumeRAM->getOwner());
        dvec2 dataRange = volume->dataMap_.dataRange;

        double delta = (dataRange.y - dataRange.x) / static_cast<double>(maxNumberOfBins);

        switch (volumeRAM->getDataFormat()->getNumericType()) {
        case DataFormatEnums::NOT_SPECIALIZED_TYPE:
            break;
        case DataFormatEnums::FLOAT_TYPE:
            break;
        case DataFormatEnums::UNSIGNED_INTEGER_TYPE:
            if (delta < 1.0) {
                delta = 1.0;
            }
            break;
        case DataFormatEnums::SIGNED_INTEGER_TYPE:
            if (delta < 1.0) {
                delta = 1.0;
            }
            break;
        }
        return static_cast<size_t>(std::ceil(dataRange.y - dataRange.x) / delta);
    }

private:
    int sampleRate_;
    std::vector<NormalizedHistogram*>* histograms_;  // One histogram per channel in the data
    size_t maxNumberOfBins_;
};

template <typename T, size_t B, int N>
class HistogramCaluculator {
public:
    static void calculate(std::vector<NormalizedHistogram*>* histograms, const VolumeRAM* volumeRAM,
                          int sampleRate, size_t numberOfBinsInHistogram) {
        // Calculate histogram data

        if (histograms->empty()) {
            histograms->push_back(new NormalizedHistogram(numberOfBinsInHistogram));
        } else {
            histograms->at(0)->resize(numberOfBinsInHistogram);
        }

        NormalizedHistogram* histogram = histograms->at(0);

        std::vector<double>* histData = histogram->getData();
        T* src = const_cast<T*>(reinterpret_cast<const T*>(volumeRAM->getData()));
        const uvec3& dim = volumeRAM->getDimension();

        const Volume* volume = reinterpret_cast<const Volume*>(volumeRAM->getOwner());
        dvec2 dataRange = volume->dataMap_.dataRange;

        size_t size = dim.x * dim.y * dim.z;

        double min = std::numeric_limits<double>::max();
        double max = std::numeric_limits<double>::min();
        double sum = 0;
        double sum2 = 0;
        double count = 0;
        uvec3 pos(0);

        for (pos.z = 0; pos.z < dim.z; pos.z += sampleRate) {
            for (pos.y = 0; pos.y < dim.y; pos.y += sampleRate) {
                for (pos.x = 0; pos.x < dim.x; pos.x += sampleRate) {
                    if (volumeRAM->shouldStopHistogramCalculation()) break;
                    double val = static_cast<double>(src[VolumeRAM::posToIndex(pos, dim)]);
                    min = std::min(min, val);
                    max = std::max(max, val);
                    sum += val;
                    sum2 += val * val;
                    count++;

                    size_t ind =
                        static_cast<size_t>((val - dataRange.x) / (dataRange.y - dataRange.x) *
                                            (numberOfBinsInHistogram - 1));

                    if (ind >= 0 && ind < numberOfBinsInHistogram) {
                        histData->at(ind)++;
                    } else {
                        //Think here
                    }
                }
            }
        }

        histogram->dataRange_ = dataRange;
        histogram->stats_.min = min;
        histogram->stats_.max = max;
        histogram->stats_.mean = sum / count;
        histogram->stats_.standardDeviation =
            std::sqrt((count * sum2 - sum * sum) / (count * (count - 1)));

        histogram->calculatePercentiles();
        histogram->performNormalization();
        histogram->calculateHistStats();
        histogram->setValid(true);
    };
};

template <typename T, size_t B, int N>
class HistogramCaluculator<glm::detail::tvec2<T, glm::defaultp>, B, N> {
public:
    static void calculate(std::vector<NormalizedHistogram*>* histograms, const VolumeRAM* volumeRAM,
                          int sampleRate, size_t numberOfBinsInHistogram) {};
};
template <typename T, size_t B, int N>
class HistogramCaluculator<glm::detail::tvec3<T, glm::defaultp>, B, N> {
public:
    static void calculate(std::vector<NormalizedHistogram*>* histograms, const VolumeRAM* volumeRAM,
                          int sampleRate, size_t numberOfBinsInHistogram) {};
};
template <typename T, size_t B, int N>
class HistogramCaluculator<glm::detail::tvec4<T, glm::defaultp>, B, N> {
public:
    static void calculate(std::vector<NormalizedHistogram*>* histograms, const VolumeRAM* volumeRAM,
                          int sampleRate, size_t numberOfBinsInHistogram) {};
};

template <typename T>
class VolumeRAMPrecision;

template <typename T, size_t B>
class VolumeRAMCustomPrecision;

template <typename T, size_t B>
void VolumeRAMNormalizedHistogram::evaluate() {
    const VolumeRAMPrecision<T>* volumeRAM =
        dynamic_cast<const VolumeRAMPrecision<T>*>(getInputVolume());

    if (!volumeRAM || !volumeRAM->getOwner()) {
        setOutput(NULL);
        return;
    }

    size_t numberOfBinsInHistogram = calculateBins(volumeRAM, maxNumberOfBins_);

    // Create Normalized Histogram
    if (!histograms_) {
        histograms_ = new std::vector<NormalizedHistogram*>();
    }

    switch (volumeRAM->getDataFormat()->getComponents()) {
        case 1:
            HistogramCaluculator<T, B, 1>::calculate(histograms_, volumeRAM, sampleRate_,
                                                     numberOfBinsInHistogram);
            break;
        case 2:
            break;
        case 3:
            break; 
        case 4: 
            break; 
        default:
            break;
    }

    if (volumeRAM->shouldStopHistogramCalculation()) {
        for (std::vector<NormalizedHistogram*>::iterator it = histograms_->begin();
             it != histograms_->end(); ++it) {
            delete *it;
        }
        histograms_->clear();
        delete histograms_;
        histograms_ = NULL;
    }

    setOutput(histograms_);
}

}  // namespace

#endif  // IVW_VOLUMERAMHISTOGRAM_H
