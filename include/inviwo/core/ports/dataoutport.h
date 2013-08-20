#ifndef IVW_DATAOUTPORT_H
#define IVW_DATAOUTPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/outport.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API DataOutport : public Outport {

public:
    DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~DataOutport();

    void initialize();
    void deinitialize();

    virtual T* getData();
    virtual const T* getConstData() const;
    void setData(T* data);

    bool hasData() const;

protected:
    T* data_;
};

template <typename T>
DataOutport<T>::DataOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : Outport(identifier, invalidationLevel),
      data_(NULL)
{
}

template <typename T>
DataOutport<T>::~DataOutport() {
    delete data_;
}

template <typename T>
void DataOutport<T>::initialize(){}

template <typename T>
void DataOutport<T>::deinitialize(){}

template <typename T>
T* DataOutport<T>::getData() {
    return data_;
}

template <typename T>
const T* DataOutport<T>::getConstData() const{
    return const_cast<const T*>(data_);
}

template <typename T>
void DataOutport<T>::setData(T* data) {
    //Delete old data
    delete data_;
    //Add reference to new data
    data_ = data;
}

template <typename T>
bool DataOutport<T>::hasData() const {
    return (data_ != NULL);
}

} // namespace

#endif // IVW_DATAOUTPORT_H
