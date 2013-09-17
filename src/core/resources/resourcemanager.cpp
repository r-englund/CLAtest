#include <inviwo/core/resources/resourcemanager.h>
#include <algorithm>
namespace inviwo {

struct ResourceComparer {
    ResourceComparer(const std::string& identifier) : identifier_(identifier) {}
    bool operator()(const Resource* resource) const {
        return *resource == identifier_;
    }
    const std::string& identifier_;
};

ResourceManager::~ResourceManager() {
    clearAllResources();
}

Resource* ResourceManager::getResource( const std::string& identifier )
{
    std::vector<Resource*>::iterator it = std::find_if(resources_.begin(), resources_.end(), ResourceComparer(identifier));
    if(it != resources_.end()) {
        return *it;
    } else {
        return NULL;
    }
}

void ResourceManager::clearAllResources(){
    // Deallocate resources
    for(std::vector<Resource*>::iterator it = resources_.begin(); it != resources_.end(); ++it) {
        notifyResourceRemoved(*it);
        delete *it;
    }
    resources_.clear();
}


} // namespace
