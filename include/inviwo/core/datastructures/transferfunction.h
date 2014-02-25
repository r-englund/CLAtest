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
 * Main file authors: Viktor Axelsson, Timo Ropinski
 *
 *********************************************************************************/

/** \Class for holding transfer function data
 *
 *  This class holds transfer function data, currently one parameter in the variable data_.
 */

#ifndef IVW_TRANSFERFUNCTION_H
#define IVW_TRANSFERFUNCTION_H

#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/transferfunctiondatapoint.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API TransferFunctionObserver: public Observer {
public:
    TransferFunctionObserver(): Observer() {};

    /**
    * This method will be called when observed object changes.
    * Override it to add behavior.
    */
    virtual void onTransferFunctionChange() {};
};
class IVW_CORE_API TransferFunctionObservable: public Observable<TransferFunctionObserver> {
public:
    TransferFunctionObservable(): Observable<TransferFunctionObserver>() {};

    void notifyTransferFunctionObservers() const {
        // Notify observers
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<TransferFunctionObserver*>(*it)->onTransferFunctionChange();
        }
    }
};

class IVW_CORE_API TransferFunction : public TransferFunctionObservable {
public:

    enum InterpolationType {
        InterpolationLinear = 0,
        InterpolationCubic
    };

    TransferFunction();
    TransferFunction(const TransferFunction& rhs);
    TransferFunction& operator=(const TransferFunction& rhs);
    virtual ~TransferFunction();

    const Layer* getData() const { return data_; }
    size_t getNumDataPoints() const { return dataPoints_.size(); }
    int getTextureSize() { return textureSize_; }

    TransferFunctionDataPoint* getPoint(int i) const;

    void addPoint(const vec2& pos, const vec4& color);
    void addPoint(TransferFunctionDataPoint* dataPoint);
    void removePoint(TransferFunctionDataPoint* dataPoint);

    void clearPoints();

    void calcTransferValues();

    float getMaskMin() { return maskMin_; }
    void setMaskMin(float maskMin) { maskMin_ = maskMin; }
    float getMaskMax() { return maskMax_; }
    void setMaskMax(float maskMax) { maskMax_ = maskMax; }

    void setInterpolationType(InterpolationType interpolationType) { interpolationType_ = interpolationType; }
    InterpolationType getInterpolationType() const { return interpolationType_; }

private:
    float maskMin_;
    float maskMax_;
    int textureSize_;
    Layer* data_;
    std::vector<TransferFunctionDataPoint*> dataPoints_;
    InterpolationType interpolationType_;
};


} // namespace
#endif // IVW_TRANSFERFUNCTION_H
