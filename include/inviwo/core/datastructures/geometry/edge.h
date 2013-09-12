#ifndef IVW_EDGE_H
#define IVW_EDGE_H

namespace inviwo {

class IVW_CORE_API Edge {

public:
	unsigned int v1, v2;
	bool operator==(const Edge& e) const {
		return ( (this->v1==e.v1) && (this->v2==e.v2) ) || ( (this->v1==e.v2) && (this->v2==e.v1) );
	}
private:

};

} // namespace

#endif // IVW_EDGE_H