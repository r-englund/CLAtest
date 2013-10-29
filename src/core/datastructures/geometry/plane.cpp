#include <inviwo/core/datastructures/geometry/plane.h>

namespace inviwo {

Plane::Plane() : 
	point_(vec3(0.0f,0.0f,0.0f)),
	normal_(vec3(0.0f,1.0f,0.0f)) {
	}

Plane::Plane(vec3 point, vec3 normal) : 
	point_(point),
	normal_( glm::normalize(normal) ) {
	}

Plane::~Plane() {
}

vec3 Plane::getIntersection(const vec3 &p1, const vec3 &p2) const {
	if(glm::abs(glm::dot(p2-p1,normal_)) < 0.0001) // Should never happen
		std::cout<<"Line parallel with clip plane!\n";

	float dist;
	vec3 l = p2 - p1;

	float nom = glm::dot(point_ - p1, normal_);
	float denom = glm::dot(l, normal_);

	dist = nom/denom;
	vec3 res = vec3(dist*l + p1);

	return vec3(dist*l + p1);
}

vec3 Plane::projectPoint(const vec3& p1) const{
    float dist = glm::dot(p1 - point_, normal_);
    return p1 - dist*normal_;
}

bool Plane::isInside(const vec3 &p) const {
	return (glm::dot(normal_,p-point_) > 0.0f) ? true : false;
}

void Plane::setPoint(const vec3 p) {
	this->point_ = p;
}

void Plane::setNormal(const vec3 &n) {
	this->normal_ = n;
}

} // namespace