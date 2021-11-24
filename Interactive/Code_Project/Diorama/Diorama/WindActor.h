#pragma once

#ifndef WIND_ACTOR_H
#define WIND_ACTOR_H

#include "Entity.h"
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "MeshComponent.h"
#include "WindComponent.h"

class WindActor : public Entity
{
public:
	explicit WindActor(const std::string& filename, 
		const glm::vec3& wind = { 0, 0, 0 }, 
		const glm::vec3& position = { 0, 0, 0 },
		bool castShadow = true) : Entity()
	{
		this->AddComponent<TransformComponent>(position);
		this->AddComponent<MeshComponent>(filename, castShadow);
		this->AddComponent<WindComponent>(wind);
	}
};

#endif // !WIND_ACTOR_H