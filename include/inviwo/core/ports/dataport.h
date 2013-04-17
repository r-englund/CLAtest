#ifndef IVW_DATAPORT_H
#define IVW_DATAPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>

#include <inviwo/core/ports/port.h>
#include <inviwo/core/datastructures/data.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API DataPort : public Port {

public:
    DataPort(PortDirection direction, std::string identifier);
    virtual ~DataPort();

    virtual void connectTo(Port* port);
    virtual void disconnectFrom(Port* port);

    virtual const T* getData() const;
    virtual T* getEditableData();
    void setData(T* data);

    bool hasData() const;

protected:
    T* data_;
    DataPort* connectedDataPort_;
};


template <typename T>
DataPort<T>::DataPort(PortDirection direction, std::string identifier)
    : Port(direction, identifier),
      data_(0)
{
    connectedDataPort_ = 0;
}

template <typename T>
DataPort<T>::~DataPort() {}

template <typename T>
void DataPort<T>::connectTo(Port* port) {
    ivwAssert(dynamic_cast<DataPort*>(port)!=0, "Trying to connect different port types.")
    Port::connectTo(port);
    if (isInport())
        connectedDataPort_ = dynamic_cast<DataPort<T>* >(port);
}

template <typename T>
void DataPort<T>::disconnectFrom(Port* port) {
    ivwAssert(dynamic_cast<DataPort*>(port)!=0, "Trying to disconnect different port types.")
    Port::disconnectFrom(port);
    if (isInport())
        connectedDataPort_ = 0;
}

template <typename T>
const T* DataPort<T>::getData() const {
    if (isOutport()){
        return const_cast<const T*>(data_);
    }
    else if (isConnected()) {
        return connectedDataPort_->getData();
    }
    else 
        return 0;
}

template <typename T>
T* DataPort<T>::getEditableData() {
    ivwAssert(isOutport(), "Calling getEditableData() on inport.");
    return data_;
}

template <typename T>
void DataPort<T>::setData(T* data) {
    ivwAssert(isOutport(), "Calling setData() on inport.");
    data_ = data;
}

template <typename T>
bool DataPort<T>::hasData() const {
    return (data_ != NULL);
}

} // namespace

#endif // IVW_DATAPORT_H
