#ifndef IVW_RESOURCEINFOCONTAINER_H
#define IVW_RESOURCEINFOCONTAINER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/util/resourceinfo.h>

namespace inviwo {

    class IVW_CORE_API ResourceInfoContainer {

    public:
        ResourceInfoContainer();
        virtual ~ResourceInfoContainer();

        //Representations
        template<class T>
        bool hasInfo() const;
        template<class T>
        T* getInfo();
        void addInfo(ResourceInfo* info);
        void printInfos();
        void clearInfo();

    protected:
        std::vector<ResourceInfo*> infos_;

    };

    template<class T>
    T* ResourceInfoContainer::getInfo() {
        if (infos_.size() > 0) {
            // check if info exists and return it
            for (size_t i=0; i<infos_.size(); i++) {
                T* representation = dynamic_cast<T*>(infos_[i]);
                if (representation) {
                    return representation;
                }
            }
        }
        return NULL;
    }

    template<class T>
    bool ResourceInfoContainer::hasInfo() const {
        for (size_t i=0; i<infos_.size(); i++) {
            T* representation = dynamic_cast<T*>(infos_[i]);
            if (representation) return true;
        }
        return false;
    }

} // namespace

#endif // IVW_RESOURCEINFOCONTAINER_H
