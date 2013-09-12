#ifndef IVW_PLANE_H
#define IVW_PLANE_H

namespace inviwo {

class IVW_CORE_API Plane {

public:
	Plane();
	Plane(glm::vec3 point, glm::vec3 normal);
	virtual ~Plane();

	glm::vec3 getIntersection(const glm::vec3&, const glm::vec3&);
	bool isInside(const glm::vec3&);
	void setPoint(const glm::vec3);
	void setNormal(const glm::vec3&);
private:
	glm::vec3 point_;
	glm::vec3 normal_;

};

} // namespace

#endif // IVW_PLANE_H