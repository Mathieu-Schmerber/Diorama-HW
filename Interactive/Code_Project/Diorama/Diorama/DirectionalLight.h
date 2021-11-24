#pragma once

#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Entity.h"
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class DirectionalLight : public Entity
{
public:
	explicit DirectionalLight(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& direction = { 0, 0, 0 }) : Entity()
	{
		this->AddComponent<TransformComponent>(position, direction);
	}
};

#endif // !DIRECTIONAL_LIGHT_H