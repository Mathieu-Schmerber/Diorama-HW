#include "CameraSystem.h"
#include "Core.h"
#include "CameraComponent.h"
#include "InputComponent.h"

void CameraSystem::CameraView(Entity* entity)
{
	Display* display = Core::Instance().GetDisplay();
	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	CameraComponent* camera = entity->GetComponent<CameraComponent>();
	float aspectRatio = (float)display->GetWidth() / (float)display->GetHeight();
	float fovy = 45.0f;

	// Setup camera
	glm::mat4 viewMatrix = glm::lookAt(transform->position(), transform->position() + transform->rotation(),  transform->up());
	glm::mat4 proj_matrix = glm::perspective(glm::radians(fovy), aspectRatio, 0.1f, 1000.0f);

	camera->SetViewMatrix(viewMatrix);
	camera->SetProjectionMatrix(proj_matrix);
}

void CameraSystem::CameraControl(Entity* entity, float deltatime)
{
	Display* display = Core::Instance().GetDisplay();
	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	InputComponent* input = entity->GetComponent<InputComponent>();
	float speed = 6.0f;
	float sensitivity = 100.0f;
	glm::vec3 movement = glm::vec3(0, 0, 0);

	// Handles key inputs
	if (input->IsKeyPressed(CONTROL::FORWARD))
		movement += transform->rotation();
	if (input->IsKeyPressed(CONTROL::BACKWARD))
		movement -= transform->rotation();
	if (input->IsKeyPressed(CONTROL::LEFT))
		movement -= glm::normalize(glm::cross(transform->rotation(), transform->up()));
	if (input->IsKeyPressed(CONTROL::RIGHT))
		movement += glm::normalize(glm::cross(transform->rotation(), transform->up()));
	if (input->IsKeyPressed(CONTROL::UPWARD))
		movement += transform->up();
	if (input->IsKeyPressed(CONTROL::DOWNWARD))
		movement -= transform->up();

	if (glm::length(movement) > 0)
		transform->MoveBy(glm::normalize(movement) * speed * deltatime);

	// Mouse inputs
	glm::vec2 mouse = input->mouse();

	float rotX = sensitivity * (float)(mouse.y - (display->GetHeight() / 2)) / display->GetHeight();
	float rotY = sensitivity * (float)(mouse.x - (display->GetWidth() / 2)) / display->GetWidth();

	glm::vec3 newOrientation = glm::rotate(transform->rotation(), glm::radians(-rotX), glm::normalize(glm::cross(transform->rotation(), transform->up())));

	// Prevent full 360 degree flip
	if (abs(glm::angle(newOrientation, transform->up()) - glm::radians(90.0f)) <= glm::radians(85.0f))
		transform->SetRotation(glm::degrees(newOrientation));

	transform->SetRotation(glm::degrees(glm::rotate(transform->rotation(), glm::radians(-rotY), transform->up())));
	input->SetCursorPosition((float)display->GetWidth() / 2, (float)display->GetHeight() / 2);
}

CameraSystem::CameraSystem() : System()
{
	this->AddDependency<TransformComponent>();
	this->AddDependency<CameraComponent>();
	this->AddDependency<InputComponent>();
}

void CameraSystem::LoadSystem()
{
	Display* display = Core::Instance().GetDisplay();
	InputComponent* input;

	for (auto& e : this->_entities)
	{
		input = e->GetComponent<InputComponent>();
		CameraView(e);

		glfwSetCursor(display->GetWindow(), glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));
		input->SetCursorPosition((float)display->GetWidth() / 2, (float)display->GetHeight() / 2);
	}
}

void CameraSystem::Update(float deltatime, float time)
{
	for (auto& e : this->_entities)
	{
		CameraView(e);
		CameraControl(e, deltatime);
	}
}