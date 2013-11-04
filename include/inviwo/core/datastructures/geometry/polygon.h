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

    virtual ~Polygon(){
        delete[] list_;
    }

    T at(size_t i) const{
        return list_[i];
    }

    size_t size() const{
        return size_;
    }

    T* values(){
        return list_;
    }

private:
    T* list_;
    size_t size_;
};

template<typename T, size_t B>
class IVW_CORE_API PolygonDataFormat :  public Polygon<T> {
    PolygonDataFormat(size_t size) : Polygon<T>(size), dataFormat_(DataFormat<T,B>::get()) {
    }
    virtual ~PolygonDataFormat(){
    }

private:
    const DataFormat<T,B>* dataFormat_;
};

#define DataFormatPolygon(D) PolygonDataFormat<D::type, D::bits>

} // namespace

#endif // IVW_POLYGON_H