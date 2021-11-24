#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "Entity.h"
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "MeshComponent.h"
#include "ButtonComponent.h"
#include "InputComponent.h"
#include "SphereColliderComponent.h"

class Button : public Entity
{
public:
	explicit Button(const std::string& file, const glm::vec3& position, TransformComponent *indicator, float radius, float factor, void (*onclick)(float)) : Entity()
	{
		this->AddComponent<TransformComponent>(position);
		this->AddComponent<MeshComponent>(file, false);
		this->AddComponent<ButtonComponent>(indicator, factor, onclick);
		this->AddComponent<InputComponent>();
		this->AddComponent<SphereColliderComponent>(position + glm::vec3(0, radius * 2, 0), radius);
	}
};

#endif // !BUTTON_H