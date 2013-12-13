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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_MULTI_DATA_INPORT_H
#define IVW_MULTI_DATA_INPORT_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/multiinport.h>
#include <set>

namespace inviwo {

template<typename T>
class DataOutport;

/** \class MultiDataInport 
*
* Port for handling multiple inports. 
* First template argument is the data contained within
* the ports and the second one is the port class. 
* Usage example:
* MultiDataInport<Image, ImagePort> multiImagePort;
* MultiDataInport<float> multiFloatPort;
* @see MultiInport
*/
template < typename T, typename U = DataInport<T> >
class MultiDataInport : public MultiInport {

public:
    typedef std::set< U* > DataInportSet;

    MultiDataInport(std::string identifier);
    virtual ~MultiDataInport();

    virtual bool canConnectTo(Port* port) const {     
        if (dynamic_cast<DataOutport<T>*>(port))
            return true;
        else
            return false; 
    };
    virtual void connectTo(Outport* outport);
    
    uvec3 getColorCode() const { return U::colorCode; }

    std::vector<const T*> getData() const;

    bool hasData() const;

};


template<typename T, typename U>
MultiDataInport<T, U>::MultiDataInport(std::string identifier)
    : MultiInport(identifier) {
}

template <typename T, typename U>
MultiDataInport<T, U>::~MultiDataInport() { 

}

template <typename T, typename U>
void MultiDataInport<T, U>::connectTo(Outport* outport) {
    DataOutport<T>* dataPort = dynamic_cast<DataOutport<T>*>(outport);
    ivwAssert(dataPort!=NULL, "Trying to connect incompatible ports.")
    // U is a Port class
    U* inport = new U(getIdentifier());
    inports_->insert(inport);
    setProcessorHelper(inport, getProcessor());
    inport->connectTo(outport);
}

template < typename T, typename U /*= DataInport<T> */>
std::vector<const T*> inviwo::MultiDataInport<T, U>::getData() const {
    std::vector<const T*> data;
    InportSet::const_iterator it = inports_->begin(); InportSet::const_iterator endIt = inports_->end();
    for(; it != endIt; ++it) {
        data.push_back(static_cast<U*>(*it)->getData());
    }
    return data;
}

template < typename T, typename U /*= DataInport<T> */>
bool inviwo::MultiDataInport<T, U>::hasData() const {
    if (isConnected()) {
        InportSet::const_iterator it = inports_->begin(); InportSet::const_iterator endIt = inports_->end();
        for(; it != endIt; ++it) {
            if(!static_cast<U*>(*it)->hasData()) 
                return false;
        }
        return true;
    } else {
        return false;
    }
}






} // namespace

#endif // IVW_MULTI_DATA_INPORT_H