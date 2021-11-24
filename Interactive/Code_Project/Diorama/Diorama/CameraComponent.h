#pragma once

#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "Component.h"

class CameraComponent : public Component
{
private:
	glm::mat4 _viewMatrix;
	glm::mat4 _projectionMatrix;

public:
	CameraComponent() : Component() {}

	glm::mat4 SetViewMatrix(glm::mat4 value)
	{
		return this->_viewMatrix = value;
	}

	glm::mat4 SetProjectionMatrix(glm::mat4 value)
	{
		return this->_projectionMatrix = value;
	}

	glm::mat4 viewMatrix() const
	{
		return this->_viewMatrix;
	}

	glm::mat4 projectionMatrix() const
	{
		return this->_projectionMatrix;
	}
};

#endif // !CAMERA_COMPONENT_H