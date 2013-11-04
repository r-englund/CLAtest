#ifndef IVW_EDGE_H
#define IVW_EDGE_H

namespace inviwo {

template<typename T>
class IVW_CORE_API Edge {

public:
    T v1, v2;

    Edge(T in1) : v1(in1), v2(in1) {};

	Edge(T in1, T in2) : v1(in1), v2(in2) {};
	
	bool operator==(const Edge<T>& e) const {
		return ( (this->v1==e.v1) && (this->v2==e.v2) ) || ( (this->v1==e.v2) && (this->v2==e.v1) );
	}

protected:
    //Protected as it can't instantiate v1 and v2;
    Edge(){}

};

template<typename T, size_t B>
class IVW_CORE_API EdgeDataFormat : public Edge<T> {

public:
    EdgeDataFormat() : Edge<T>(), dataFormat_(DataFormat<T,B>::get()) {
        dataFormat_->floatToValue(0.f, &v1);
        v2 = v1;
    }

    EdgeDataFormat(T in1) : Edge<T>(in1), dataFormat_(DataFormat<T,B>::get()) {};
    EdgeDataFormat(T in1, T in2) : Edge<T>(in1, in2), dataFormat_(DataFormat<T,B>::get()) {};

private:
    const DataFormat<T,B>* dataFormat_;

};

#define DataFormatEdge(D) EdgeDataFormat<D::type, D::bits>

typedef DataFormatEdge(DataUINT32) EdgeIndex;
typedef DataFormatEdge(DataVec2FLOAT32) Edge2D;
typedef DataFormatEdge(DataVec3FLOAT32) Edge3D;

} // namespace

#endif // IVW_EDGE_H