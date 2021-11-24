#pragma once

#ifndef SPHERE_COLLIDER_COMPONENT_H
#define SPHERE_COLLIDER_COMPONENT_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>
#include<glm/gtx/intersect.hpp>

#include "Component.h"

class SphereColliderComponent : public Component
{
private:
	float _radius;
	glm::vec3 _center;

public:
	explicit SphereColliderComponent(const glm::vec3& center = {0, 0, 0}, const float& radius = 0) 
		: _center(center), _radius(radius), Component() {};

	bool Collide(glm::vec3 start, glm::vec3 dir, float distance)
	{
		return glm::intersectRaySphere(start, glm::normalize(dir), _center, _radius, distance);
	}
};

#endif // !SPHERE_COLLIDER_COMPONENT_H