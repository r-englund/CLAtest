#ifndef IVW_EDGE_H
#define IVW_EDGE_H

namespace inviwo {

class IVW_CORE_API Edge {

public:
	Edge() { v2 = v1 = -1; };
	Edge(unsigned int in1, unsigned int in2) : 
		v1(in1), v2(in2) {};
	unsigned int v1, v2;
	bool operator==(const Edge& e) const {
		return ( (this->v1==e.v1) && (this->v2==e.v2) ) || ( (this->v1==e.v2) && (this->v2==e.v1) );
	}
private:

};

} // namespace

#endif // IVW_EDGE_H