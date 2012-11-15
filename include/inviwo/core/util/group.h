#ifndef IVW_GROUP_H
#define IVW_GROUP_H

#include <map>

template<typename KEY,typename DATA>
class Group {
public:

    Group(){}
    ~Group() {}

    void deinitialize() {
        std::vector<KEY> keys = getGroupKeys();
        for(size_t i=0; i<keys.size(); i++) {
            groupMap_.erase(keys[i]);
        }
    }

    std::vector<DATA> getGroupedData(KEY groupKEY) {
        std::pair<GroupMap::iterator, GroupMap::iterator> pgRangeIt;
        std::vector<DATA> ports;
        pgRangeIt = groupMap_.equal_range(groupKEY);
        for (GroupMap::iterator mIt = pgRangeIt.first; mIt != pgRangeIt.second; ++mIt) {
            ports.push_back((*mIt).second);
        }
        return ports;
    }

    std::vector<KEY> getGroupKeys() {        
        std::map<KEY, int> keyMap;
        std::map<KEY, int>::iterator keyMapIt;
        std::vector<KEY> groups;
        if (groupMap_.empty()) return groups;
        GroupMap::iterator it;
        for (it = groupMap_.begin(); it != groupMap_.end(); ++it) {
            keyMap[(*it).first]++;
        }
        for (keyMapIt=keyMap.begin(); keyMapIt != keyMap.end(); keyMapIt++) {
            groups.push_back((*keyMapIt).first);
        }
        return groups;
    }

    KEY getKey(DATA data) {
        KEY key;
        GroupMap::iterator it;
        for (it = groupMap_.begin(); it != groupMap_.end(); ++it) {
            if ((*it).second == data) {
                key = (*it).first;
                //break;
            }
        }    
        return key;
    }

    void insert(KEY key, DATA data) {
        groupMap_.insert(std::pair<KEY,DATA>(key, data));
    }

private:
    typedef std::multimap<KEY,DATA> GroupMap;
    GroupMap groupMap_;
};

#endif
