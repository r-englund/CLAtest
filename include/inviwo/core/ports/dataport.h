#ifndef IVW_DATAPORT_H
#define IVW_DATAPORT_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/port.h"
#include "inviwo/core/datastructures/data.h"


namespace inviwo {

template<typename T>
class IVW_CORE_API DataPort : public Port {

public:

    DataPort(PortDirection direction, std::string name);
    virtual ~DataPort();

    virtual void connectTo(Port* port);
    virtual void disconnectFrom(Port* port);

    virtual T* getData() ;
    void setData(T* data);

protected:
    T* data_;
    DataPort* connectedDataPort_;
};


template <typename T>
DataPort<T>::DataPort(PortDirection direction, std::string identifier)
    : Port(direction, identifier), data_(0)
{}

template <typename T>
DataPort<T>::~DataPort() {}

template <typename T>
IVW_CORE_API void DataPort<T>::connectTo(Port* port) {
    Port::connectTo(port);
    //TODO: check that port is a DataPort
    connectedDataPort_ = dynamic_cast<DataPort<T>* >(port);
}

template <typename T>
IVW_CORE_API void DataPort<T>::disconnectFrom(Port* port) {
    Port::disconnectFrom(port);
    //TODO: check that port is a DataPort
    connectedDataPort_ = 0;
}

template <typename T>
IVW_CORE_API T* DataPort<T>::getData() {
    if (isOutport()) return data_;
    else if (isConnected()) {
        return connectedDataPort_->getData();
    }
    else return 0;
}

template <typename T>
IVW_CORE_API void DataPort<T>::setData(T* data) {
    data_ = data;
}

} // namespace

#endif // IVW_DATAPORT_H
