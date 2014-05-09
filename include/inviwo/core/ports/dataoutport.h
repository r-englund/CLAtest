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

#ifndef IVW_DATAOUTPORT_H
#define IVW_DATAOUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/outport.h>

namespace inviwo {

template< typename T, typename U> class MultiDataInport;

template< typename T> class DataInport;

template<typename T>
class DataOutport : public Outport {

    friend class MultiDataInport<T, DataInport<T> >;
public:
    DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~DataOutport();

    void initialize();
    void deinitialize();

    virtual T* getData();
    virtual const T* getConstData() const;

    void setData(T* data, bool ownsData = true);
    void setConstData(const T* data);

    bool hasData() const;
    bool isDataOwner() const;
    virtual std::string getContentInfo() const;

    virtual void dataChanged() {}

protected:
    T* data_;
    bool ownsData_;
};

template <typename T>
DataOutport<T>::DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : Outport(identifier, invalidationLevel),
    data_(NULL), ownsData_(false)
{
}

template <typename T>
DataOutport<T>::~DataOutport() {
    if (ownsData_ && data_)
        delete data_;
}

template <typename T>
void DataOutport<T>::initialize() {}

template <typename T>
void DataOutport<T>::deinitialize() {}

template <typename T>
T* DataOutport<T>::getData() {
    return data_;
}

template <typename T>
const T* DataOutport<T>::getConstData() const {
    return const_cast<const T*>(data_);
}

template <typename T>
void DataOutport<T>::setData(T* data, bool ownsData) {
    if (ownsData_ && data_) {
        //Delete old data
        delete data_;
    }

    ownsData_ = ownsData;
    //Add reference to new data
    data_ = data;
    dataChanged();
}

template <typename T>
void DataOutport<T>::setConstData(const T* data) {
    if (ownsData_ && data_) {
        //Delete old data
        delete data_;
    }

    ownsData_ = false;
    //Add reference to new data
    data_ = const_cast<T*>(data);
    dataChanged();
}

template <typename T>
bool DataOutport<T>::hasData() const {
    return (data_ != NULL);
}

template <typename T>
bool DataOutport<T>::isDataOwner() const {
    return ownsData_;
}

template <typename T>
std::string DataOutport<T>::getContentInfo() const {
    if (hasData()) {
        const BaseData* data = dynamic_cast<const BaseData*>(data_);
        if (data) {
            return data->getDataInfo();
        } else {
            return "Not a BaseData Object";
        }
    } else {
        return getClassName() + " has no data.";
    }
}

} // namespace

#endif // IVW_DATAOUTPORT_H
