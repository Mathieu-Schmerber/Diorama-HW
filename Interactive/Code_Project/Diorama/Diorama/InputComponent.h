#pragma once

#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "Component.h"

#include <GLFW\glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Core;

enum CONTROL {
	FORWARD = GLFW_KEY_W,
	BACKWARD = GLFW_KEY_S,
	LEFT = GLFW_KEY_A,
	RIGHT = GLFW_KEY_D,
	UPWARD = GLFW_KEY_SPACE,
	DOWNWARD = GLFW_KEY_LEFT_CONTROL
};

enum MOUSE {
	LEFT_CLICK = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT_CLICK = GLFW_MOUSE_BUTTON_RIGHT
};

class InputComponent : public Component
{
private:
	glm::vec2 _mousePosition;
	bool _rightClickHold = false;
	bool _leftClickHold = false;

public:

	explicit InputComponent();

	bool IsKeyPressed(enum CONTROL key);
	bool IsMousePressed(enum MOUSE key);
	bool IsMouseReleased(enum MOUSE key);

	bool IsHoldingClick(MOUSE key);
	void SetHoldingClick(MOUSE key, bool hold);

	void SetCursorVisibility(bool visibility);
	void SetCursorPosition(float x, float y);

	glm::vec2 mouse();
};

#endif // !INPUT_COMPONENT_H