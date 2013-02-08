#ifndef IVW_VECTOROPERATIONS_H
#define IVW_VECTOROPERATIONS_H

#include <inviwo/core/inviwocoredefine.h>
#include <vector>

namespace inviwo {

    template<class T, class U>
    IVW_CORE_API T* getTypeFromVector(std::vector<U> vec) {
        if (vec.size() > 0) {
            for (size_t i=0; i<vec.size(); i++) {
                T* item = dynamic_cast<T*>(vec[i]);
                if (item) {
                    return item;
                }
            }
        }
        return NULL;
    }

    template<class T, class U>
    IVW_CORE_API bool hasTypeInVector(const std::vector<U> vec) {
        for (size_t i=0; i<vec.size(); i++) {
            T* item = dynamic_cast<T*>(vec[i]);
            if (item){
                return true;
            }
        }
        return false;
    }

} // namespace

#endif // IVW_VECTOROPERATIONS_H
