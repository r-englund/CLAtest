#ifndef IVW_POLYGON_H
#define IVW_POLYGON_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/edge.h>

namespace inviwo {

/**
 * \class Polygon
 * \brief Create a Polygon which consists of points.
 *
 * A Polygon consists of a specific number of vertices, or indices to vertices. 
 * When polygon contains vertices, adjacent vertices are considered to be connected with lines to form a edge.
 * First and last vertex are also considered to be connected, to form a closed chain.
 */

template<typename T>
class IVW_CORE_API Polygon  {

public:
    Polygon(size_t size) : size_(size) {
        list_ = new T[size_];
    }

    Polygon(const Polygon& rhs) : size_(rhs.size()){
        list_ = new T[size_];
        for(size_t i=0; i < size(); ++i){
            list_[i] = rhs.get(i);
        }
    }

    virtual ~Polygon(){
        delete[] list_;
    }

    T& at(size_t i){
        return list_[i];
    }

    T get(size_t i) const{
        return list_[i];
    }

    size_t size() const{
        return size_;
    }

    const T* values() const{
        return list_;
    }

    T* values(){
        return list_;
    }

private:
    T* list_;
    size_t size_;
};

} // namespace

#endif // IVW_POLYGON_H