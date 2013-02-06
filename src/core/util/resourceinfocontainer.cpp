#include <inviwo/core/util/resourceinfocontainer.h>

namespace inviwo {

    ResourceInfoContainer::ResourceInfoContainer() {}
    ResourceInfoContainer::~ResourceInfoContainer() {}

    void ResourceInfoContainer::addInfo(ResourceInfo* info) {
        info->initialize();
        infos_.push_back(info);
    }

    void ResourceInfoContainer::clearInfo() {
        while (!infos_.empty()) {
            delete infos_.back();
            infos_.pop_back();
        }
    }

    void ResourceInfoContainer::printInfos() {
        for (size_t i=0; i<infos_.size(); i++) {
            infos_[i]->printInfo();
        }
    }

} // namespace
