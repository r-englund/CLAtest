#ifndef IVW_PLANE_H
#define IVW_PLANE_H

namespace inviwo {

class IVW_CORE_API Plane {

public:
	Plane();
	Plane(vec3 point, vec3 normal);
	virtual ~Plane();

	vec3 getIntersection(const vec3&, const vec3&);
	bool isInside(const vec3&);
	void setPoint(const vec3);
	void setNormal(const vec3&);
private:
	vec3 point_;
	vec3 normal_;

};

} // namespace

#endif // IVW_PLANE_H