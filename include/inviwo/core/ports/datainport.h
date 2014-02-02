/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_DATAINPORT_H
#define IVW_DATAINPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/singleinport.h>

namespace inviwo {

template<typename T>
class DataOutport;

template<typename T>
class DataInport : public SingleInport {
public:
    DataInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~DataInport();

    void initialize();
    void deinitialize();

    virtual bool canConnectTo(Port* port) const;
    virtual void connectTo(Outport* port);

    virtual const T* getData() const;

    bool hasData() const;

    virtual bool isReady() const { return isConnected() && hasData(); }
};


template <typename T>
DataInport<T>::DataInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : SingleInport(identifier, invalidationLevel)
{
}

template <typename T>
DataInport<T>::~DataInport() {}

template <typename T>
void DataInport<T>::initialize() {}

template <typename T>
void DataInport<T>::deinitialize() {}

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

    if (dataPort != NULL)
        SingleInport::connectTo(port);
    else
        LogWarn("Trying to connect incompatible ports.");
}

template <typename T>
const T* DataInport<T>::getData() const {
    if (isConnected()) {
        // Safe to static cast since we are unable to connect other outport types.
        return static_cast< DataOutport<T>* >(connectedOutport_)->getConstData();
    } else
        return NULL;
}

template <typename T>
bool DataInport<T>::hasData() const {
    if (isConnected()) {
        // Safe to static cast since we are unable to connect other outport types.
        return static_cast< DataOutport<T>* >(connectedOutport_)->hasData();
    } else
        return false;
}

} // namespace

#endif // IVW_DATAINPORT_H
