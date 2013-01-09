#include "inviwo/core/metadata/metadatamap.h"
#include "inviwo/core/io/serialization/ivwserializable.h"



namespace inviwo {

MetaDataMap::MetaDataMap() {}

MetaDataMap::~MetaDataMap() {}

void MetaDataMap::add(std::string key, MetaData* metaData) {
    remove(key);
    metaData_[key] = metaData;
}

void MetaDataMap::remove(std::string key) {
    if (!get(key)) return;
    metaData_.erase(metaData_.find(key));
}

void MetaDataMap::removeAll() {
    metaData_.clear();
}

void MetaDataMap::rename(std::string newKey, MetaData* metaData) {
    for (cItreator cIt = metaData_.begin(); cIt!=metaData_.end(); ++cIt) {
        if (cIt->second == metaData) {
            std::string oldKey = cIt->first;
            remove(oldKey);
            break;
        }
    }
    add(newKey, metaData);
}

void MetaDataMap::rename(std::string newKey, std::string oldKey) {
    MetaData* data = get(oldKey);
    if (data) {
        remove(oldKey);
        add(newKey, data);
    }
}

std::vector<std::string> MetaDataMap::getKeys() {
    std::vector<std::string> keys;
    for (cItreator cIt = metaData_.begin(); cIt!=metaData_.end(); ++cIt)
        keys.push_back(cIt->first);
    return keys;
}

MetaData* MetaDataMap::get(std::string key) {
    if (metaData_.find(key)!=metaData_.end())
        metaData_[key];
    return 0;
}

} // namespace
