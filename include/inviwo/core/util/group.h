#ifndef IVW_GROUP_H
#define IVW_GROUP_H

#include "inviwo/core/inviwocoredefine.h"
#include <map>

/*

1) Group can consist key and data
2) Key, for example, can be std::string, Processor* , int ... etc
3) Data, for example, can be Port*, Processor*, Canvas* ... etc

Example Group types: <std::string, Port*> where KEY is std::string, DATA is Port*
                     <std::string, Processor*> where KEY is std::string, DATA is Processor*
                     <Processor*, Port*> where KEY is Processor, DATA is Port*

*/
template<typename KEY,typename DATA>
class IVW_CORE_API Group {
public:

    Group(){}
    ~Group() {}

    //Erase all data corressponding to each key
    void deinitialize() {
        std::vector<KEY> keys = getGroupKeys();
        for(size_t i=0; i<keys.size(); i++) {
            groupMap_.erase(keys[i]);
        }
    }

    //Get the data corressponding to supplied key. There can be multiple data for each key.
    std::vector<DATA> getGroupedData(KEY groupKEY) {
        std::pair<typename GroupMap::iterator, typename GroupMap::iterator> pgRangeIt;
        std::vector<DATA> ports;
        pgRangeIt = groupMap_.equal_range(groupKEY);
        for (typename GroupMap::iterator mIt = pgRangeIt.first; mIt != pgRangeIt.second; ++mIt) {
            ports.push_back((*mIt).second);
        }
        return ports;
    }

    //Get all existing keys in map with no duplicates
    std::vector<KEY> getGroupKeys() {        
        std::map<KEY, int> keyMap;
        typename std::map<KEY, int>::iterator keyMapIt;
        std::vector<KEY> groups;
        if (groupMap_.empty()) return groups;
        typename GroupMap::iterator it;
        for (it = groupMap_.begin(); it != groupMap_.end(); ++it) {
            keyMap[(*it).first]++;
        }
        for (keyMapIt=keyMap.begin(); keyMapIt != keyMap.end(); keyMapIt++) {
            groups.push_back((*keyMapIt).first);
        }
        return groups;
    }

    //Get key that corresponds to data
    KEY getKey(DATA data) {
        KEY key;
        typename GroupMap::iterator it;
        for (it = groupMap_.begin(); it != groupMap_.end(); ++it) {
            if ((*it).second == data) {
                key = (*it).first;
                //break;
            }
        }    
        return key;
    }

    //Insert key and data  
    void insert(KEY key, DATA data) {
        groupMap_.insert(std::pair<KEY,DATA>(key, data));
    }

private:
    typedef std::multimap<KEY,DATA> GroupMap;
    GroupMap groupMap_;
};

#endif
