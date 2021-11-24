#pragma once

#ifndef LEVITATING_ACTOR_H
#define LEVITATING_ACTOR_H

#include "Entity.h"
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "MeshComponent.h"
#include "LevitationComponent.h"

class LevitatingActor : public Entity
{
public:
	explicit LevitatingActor(const std::string& filename,
		const float& frequence = 1,
		const float& intensity = 1,
		const glm::vec3& position = { 0, 0, 0 }
		) : Entity()
	{
		this->AddComponent<TransformComponent>(position);
		this->AddComponent<MeshComponent>(filename);
		this->AddComponent<LevitationComponent>(frequence, intensity);
	}
};

#endif // !LEVITATING_ACTOR_H