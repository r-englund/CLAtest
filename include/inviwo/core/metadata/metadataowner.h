/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_METADATAOWNER_H
#define IVW_METADATAOWNER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/metadatamap.h>

namespace inviwo {
/** \brief Holds metadata and access functionality for set/get
 *
 *  MetaDataOwner is the base class for all the objects that want to own metadata.
 *
 */
class IVW_CORE_API MetaDataOwner {

public:
    MetaDataOwner();
    MetaDataOwner(const MetaDataOwner& rhs);
    MetaDataOwner& operator=(const MetaDataOwner& rhs);
    virtual ~MetaDataOwner();
 
    //MetaData
    template<typename T, typename U>
    void setMetaData(std::string key, U value);
    //param val is required to deduce the template argument
    template<typename T, typename U>
    U getMetaData(std::string key, U val) const;
    MetaDataMap* getMetaDataMap() const { return metaData_; }

protected:
    MetaDataMap* metaData_;
};

template<typename T, typename U>
void MetaDataOwner::setMetaData(std::string key, U value) {
    MetaData* baseMetaData = metaData_->get(key);

    T* derivedMetaData = 0;
    if (baseMetaData) {
        derivedMetaData = dynamic_cast<T*>(baseMetaData);
        //if not an instance of valid meta data, forcefully replace with valid one
        if (!derivedMetaData) {
            metaData_->remove(key);
            derivedMetaData = new T();
            metaData_->add(key, derivedMetaData);
        }
        derivedMetaData->set(value);
    }
    else {
        derivedMetaData = new T();
        metaData_->add(key, derivedMetaData);
        derivedMetaData->set(value);
    }
}

//param val is required to deduce the template argument
template<typename T, typename U>
U MetaDataOwner::getMetaData(std::string key, U val) const {
    const MetaData* baseMetaData = metaData_->get(key);

    const T* derivedMetaData = 0;
    if (baseMetaData) {
        derivedMetaData = dynamic_cast<const T*>(baseMetaData);
        //if not an instance of valid meta data, forcefully replace with valid one
        if (!derivedMetaData) {
            return val;
        }
        return derivedMetaData->get();
    }
    else {
        return val;
    }
}

} // namespace

#endif // IVW_METADATAOWNER_H
