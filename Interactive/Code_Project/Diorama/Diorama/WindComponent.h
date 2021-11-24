#pragma once

#ifndef WIND_COMPONENT_H
#define WIND_COMPONENT_H

#include "Component.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>

class WindComponent : public Component
{
private:
	glm::vec3 _direction;
public:
	explicit WindComponent(const glm::vec3& direction = { 0, 0, 0 }) : _direction(direction), Component() {}

	glm::vec3 direction() const { return this->_direction; }
};

#endif // !WIND_COMPONENT_H