#include <inviwo/core/datastructures/geometry/plane.h>

namespace inviwo {

Plane::Plane() : 
	point_(glm::vec3(0.0f,0.0f,0.0f)),
	normal_(glm::vec3(0.0f,1.0f,0.0f)) {
	}

Plane::Plane(glm::vec3 point, glm::vec3 normal) : 
	point_(point),
	normal_( glm::normalize(normal) ) {
	}

Plane::~Plane() {
}

glm::vec3 Plane::getIntersection(const glm::vec3 &p1, const glm::vec3 &p2) {
	if(glm::abs(glm::dot(p2-p1,normal_)) < 0.0001) // Should never happen
		std::cout<<"Line parallel with clip plane!\n";

	float dist;
	glm::vec3 l = p2 - p1;
	glm::vec3 l0 = p1;

	float nom = glm::dot(point_ - l0, normal_);
	float denom = glm::dot(l, normal_);

	dist = nom/denom;
	glm::vec3 res = glm::vec3(dist*l + l0);
	//std::cout << "p1: ("+std::to_string(p1[0])+", "+std::to_string(p1[1])+", "+std::to_string(p1[2])+")\n";
	//std::cout << "p2: ("+std::to_string(p2[0])+", "+std::to_string(p2[1])+", "+std::to_string(p2[2])+")\n";
	//std::cout << "Computed line-plane intersection at: ("+std::to_string(res[0])+", "+std::to_string(res[1])+", "+std::to_string(res[2])+")\n";
	return glm::vec3(dist*l + l0);
}

bool Plane::isInside(const glm::vec3 &p) {
	return (glm::dot(normal_,p-point_) > 0.0f) ? true : false;
}

void Plane::setPoint(const glm::vec3 p) {
	this->point_ = p;
}

void Plane::setNormal(const glm::vec3 &n) {
	this->normal_ = n;
}
/*
void setPoint(const glm::vec3&);
void setNormal(const glm::vec3&);*/

} // namespace