/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_DATASEQUENCE_H
#define IVW_DATASEQUENCE_H

#include <inviwo/core/common/inviwocoredefine.h>

namespace inviwo {

template <typename T>
class DataSequence : public T {

public:
    DataSequence();
    DataSequence(const DataSequence& rhs);
    DataSequence<T>& operator=(const DataSequence<T>& rhs);

    virtual ~DataSequence();

    void add(T*);

    void setCurrentIndex(int);
    
    T* getCurrent();

protected:
    std::vector<T*> sequenceContainer_;
    int currentIdx_;

};

template <typename T>
DataSequence<T>::DataSequence() : T(), currentIdx_(0) {}

template <typename T>
DataSequence<T>::DataSequence(const DataSequence<T>& rhs) : T(rhs), currentIdx_(rhs.currentIdx_) {}

template <typename T>
DataSequence<T>& DataSequence<T>::operator=(const DataSequence<T>& rhs){
    if (this != &rhs) {
        T::operator=(rhs);
        currentIdx_(rhs.currentIndex_);
    }
    return *this;
}

template <typename T>
DataSequence<T>::~DataSequence() {}

template <typename T>
void DataSequence<T>::add(T* d) {
    sequenceContainer_.push_back(d);
}

template <typename T>
void DataSequence<T>::setCurrentIndex(int idx) {
    currentIdx_ = idx;
}

template <typename T>
T* DataSequence<T>::getCurrent() {
    if(currentIdx_ < static_cast<int>(sequenceContainer_.size()))
        return sequenceContainer_[currentIdx_];

    if(!sequenceContainer_.empty())
        sequenceContainer_[0];

    return NULL;
}

} // namespace

#endif // IVW_DATASEQUENCE_H
