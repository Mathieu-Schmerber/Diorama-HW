#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "Entity.h"
#include "CameraComponent.h" // includes glfw
#include "InputComponent.h" // includes glfw
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>


class Camera : public Entity
{
public:
	explicit Camera(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& front = { 0, 0, 0 }) : Entity()
	{
		this->AddComponent<TransformComponent>(position, front);
		this->AddComponent<CameraComponent>();
		this->AddComponent<InputComponent>();
	}
};

#endif // !CAMERA_H