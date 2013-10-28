#ifndef IVW_POLYGON_H
#define IVW_POLYGON_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

/**
 * \class Polygon
 * \brief Create a Polygon which consists of points.
 *
 * A Polygon consists of a specific number of vertices, or indices to vertices. 
 * When polygon contains vertices, adjacent vertices are considered to be connected with lines to form a edge.
 * First and last vertex are also considered to be connected, to form a closed chain.
 */
template<typename T, size_t B, size_t N>
class IVW_CORE_API Polygon  {

public:
    Polygon() : dataFormat_(DataFormat<T,B>::get()) {
        list_ = new T[N];
    }

    virtual ~Polygon(){
        delete[] list_;
    }

    T at(size_t i) const{
        return list_[i];
    }

    size_t size() const{
        return N;
    }

    T* vertices(){
        return list_;
    }

private:
    T* list_;
    const DataFormat<T,B>* dataFormat_;
};

#define DataFormatPolygon(D, N) Polygon<D::type, D::bits, N>

typedef DataFormatPolygon(DataUINT32, 3) Triangle;
typedef DataFormatPolygon(DataVec2FLOAT32, 3) Triangle2D;
typedef DataFormatPolygon(DataVec3FLOAT32, 3) Triangle3D;

} // namespace

#endif // IVW_POLYGON_H