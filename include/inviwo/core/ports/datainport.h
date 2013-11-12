#ifndef IVW_DATAINPORT_H
#define IVW_DATAINPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/inport.h>

namespace inviwo {

template<typename T>
class DataOutport;

template<typename T>
class DataInport : public Inport {

public:
    DataInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~DataInport();

    void initialize();
    void deinitialize();

    virtual bool canConnectTo(Port* port) const;
    virtual void connectTo(Outport* port);
    virtual void disconnectFrom(Outport* port);

    virtual const T* getData() const;

    bool hasData() const;

	virtual bool isReady() const { return isConnected() && hasData(); }

protected:
    DataOutport<T>* dataOutport_;
};


template <typename T>
DataInport<T>::DataInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : Inport(identifier, invalidationLevel),
      dataOutport_(NULL)
{
}

template <typename T>
DataInport<T>::~DataInport() {}

template <typename T>
void DataInport<T>::initialize(){}

template <typename T>
void DataInport<T>::deinitialize(){}

template <typename T>
bool DataInport<T>::canConnectTo(Port* port) const {
    if (dynamic_cast<DataOutport<T>*>(port))
        return true;
    else
        return false;
}

template <typename T>
void DataInport<T>::connectTo(Outport* port) {
    DataOutport<T>* dataPort = dynamic_cast<DataOutport<T>*>(port);
    ivwAssert(dataPort!=NULL, "Trying to connect incompatible ports.")
    dataOutport_ = dataPort;
    Inport::connectTo(port);
}

template <typename T>
void DataInport<T>::disconnectFrom(Outport* port) {
    dataOutport_ = NULL;
    Inport::disconnectFrom(port);
}

template <typename T>
const T* DataInport<T>::getData() const {
    if (isConnected())
        return dataOutport_->getConstData();
    else 
        return NULL;
}

template <typename T>
bool DataInport<T>::hasData() const {
    if (isConnected())
        return dataOutport_->hasData();
    else 
        return false;
}

} // namespace

#endif // IVW_DATAINPORT_H
