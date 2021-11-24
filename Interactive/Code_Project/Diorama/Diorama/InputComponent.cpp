#include "Core.h"
#include "InputComponent.h"

InputComponent::InputComponent() : _leftClickHold(false), _rightClickHold(false), Component()
{
}

bool InputComponent::IsMouseReleased(MOUSE key)
{
	return (glfwGetMouseButton(Core::Instance().GetDisplay()->GetWindow(), key) == GLFW_RELEASE);
}

bool InputComponent::IsHoldingClick(MOUSE key)
{
	switch (key)
	{
	case LEFT_CLICK:
		return this->_leftClickHold;
	case RIGHT_CLICK:
		return this->_rightClickHold;
	}
	return false;
}

void InputComponent::SetHoldingClick(MOUSE key, bool hold)
{
	switch (key)
	{
	case LEFT_CLICK:
		_leftClickHold = hold;
		break;
	case RIGHT_CLICK:
		_rightClickHold = hold;
		break;
	default:
		break;
	}
}

void InputComponent::SetCursorVisibility(bool visibility)
{
	glfwSetInputMode(Core::Instance().GetDisplay()->GetWindow(), GLFW_CURSOR, visibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

void InputComponent::SetCursorPosition(float x, float y)
{
	Display* display = Core::Instance().GetDisplay();

	glfwSetCursorPos(Core::Instance().GetDisplay()->GetWindow(), x, y);
}

glm::vec2 InputComponent::mouse()
{
	double x;
	double y;

	glfwGetCursorPos(Core::Instance().GetDisplay()->GetWindow(), &x, &y);
	return glm::vec2(x, y);
}

bool InputComponent::IsMousePressed(MOUSE key)
{
	return(glfwGetMouseButton(Core::Instance().GetDisplay()->GetWindow(), key) == GLFW_PRESS);
}

bool InputComponent::IsKeyPressed(CONTROL key)
{
	return (glfwGetKey(Core::Instance().GetDisplay()->GetWindow(), key) == GLFW_PRESS);
}
