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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/metadata/metadatamap.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

MetaDataMap::MetaDataMap() {}

MetaDataMap::MetaDataMap(const MetaDataMap& inMap) {
    for (cIterator cIt = inMap.metaData_.begin(); cIt!=inMap.metaData_.end(); ++cIt) {
        metaData_[cIt->first] = cIt->second->clone();
    }
}

MetaDataMap::~MetaDataMap() {}

MetaDataMap* MetaDataMap::clone() const {
    return new MetaDataMap(*this);
}

void MetaDataMap::add(std::string key, MetaData* metaData) {
    remove(key);
    metaData_[key] = metaData;
}

void MetaDataMap::remove(std::string key) {
    iterator it = metaData_.find(key);
    if (it != metaData_.end() && it->second){
        delete it->second;
        metaData_.erase(it);
    }
}

void MetaDataMap::removeAll() {
    for (cIterator cIt = metaData_.begin(); cIt!=metaData_.end(); ++cIt)
        delete cIt->second;
    metaData_.clear();
}

void MetaDataMap::rename(std::string newKey, std::string oldKey) {
    MetaData* data = get(oldKey);
    if (data) {
        metaData_.erase(oldKey);
        add(newKey, data);
    }
}

std::vector<std::string> MetaDataMap::getKeys() {
    std::vector<std::string> keys;
    for (cIterator cIt = metaData_.begin(); cIt!=metaData_.end(); ++cIt)
        keys.push_back(cIt->first);
    return keys;
}

MetaData* MetaDataMap::get(std::string key) {
    cIterator it = metaData_.find(key);
    if (it!=metaData_.end())
        return it->second;
    return NULL;
}

const MetaData* MetaDataMap::get(std::string key) const{
    cIterator it = metaData_.find(key);
    if (it!=metaData_.end())
        return const_cast<const MetaData*>(it->second);
    return NULL;
}

MetaDataMap& MetaDataMap::operator=(const MetaDataMap& map) {
    removeAll();
    for (cIterator cIt = map.metaData_.begin(); cIt!=map.metaData_.end(); ++cIt)
        metaData_[cIt->first] = cIt->second->clone();
    return *this;
}

void MetaDataMap::serialize( IvwSerializer &s ) const{
    s.serialize("MetaDataMap", metaData_, "MetaDataItem");
}

void MetaDataMap::deserialize( IvwDeserializer &d ){
    d.deserialize_test("MetaDataMap", metaData_, "MetaDataItem");
}

} // namespace
