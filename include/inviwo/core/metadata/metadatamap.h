#ifndef IVW_METADATA_MAP_H
#define IVW_METADATA_MAP_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/metadata/metadata.h>
#include <inviwo/core/metadata/metadatafactory.h>

namespace inviwo {

class IVW_CORE_API MetaDataMap {

public:
    MetaDataMap();
    MetaDataMap(const MetaDataMap& );   
    virtual ~MetaDataMap();
    void add(std::string key, MetaData* metaData);
    void remove(std::string key);
    void removeAll();
    void rename(std::string newKey, MetaData* metaData);
    void rename(std::string newKey, std::string oldKey);
    std::vector<std::string> getKeys();
    MetaData* get(std::string key);
    const MetaData* get(std::string key) const;

    MetaDataMap& operator=(const MetaDataMap& map) ;

    typedef std::map<std::string, MetaData*>::const_iterator cItreator;
    typedef std::map<std::string, MetaData*>::iterator itreator;

private:
    std::map<std::string, MetaData*> metaData_;
};

} // namespace

#endif // IVW_METADATA_MAP_H
