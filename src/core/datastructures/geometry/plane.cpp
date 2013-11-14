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

vec3 Plane::getPoint() const{
    return point_;
}

vec3 Plane::getNormal() const{
    return normal_;
}

vec3 Plane::getIntersection(const vec3 &p1, const vec3 &p2) const {
	ivwAssert(!(glm::abs(glm::dot(p2-p1,normal_)) < 0.0001f), "Line parallel with clip plane.");

	vec3 l = p2 - p1;

	float nom = glm::dot(point_ - p1, normal_);
	float denom = glm::dot(l, normal_);

    float dist = nom/denom;
    float roundDist = static_cast<float>(static_cast<int>(dist*1000000 + (dist<0.f ? -0.5f : 0.5f)))/1000000.f;

	vec3 res = vec3(roundDist*l + p1);

	return res;
}

vec3 Plane::projectPoint(const vec3& p1) const{
    float dist = glm::dot(p1 - point_, normal_);
    return p1 - dist*normal_;
}

bool Plane::isInside(const vec3 &p) const {
	return (glm::dot(normal_,p-point_) > 0.f) ? true : false;
}

bool Plane::perpendicularToPlane(const vec3&p) const{
    return (glm::abs(glm::dot(normal_, p)) < 0.0001f);
}

void Plane::setPoint(const vec3 p) {
	this->point_ = p;
}

void Plane::setNormal(const vec3 &n) {
	this->normal_ = n;
}

} // namespace