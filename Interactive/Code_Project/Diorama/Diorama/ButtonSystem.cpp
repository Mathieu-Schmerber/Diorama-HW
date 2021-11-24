#include "ButtonSystem.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ButtonComponent.h"
#include "InputComponent.h"
#include "SphereColliderComponent.h"
#include "Engine.h"

void ButtonSystem::OnClick(ButtonComponent* button, TransformComponent* transform, float deltatime)
{
	TransformComponent* indicator = button->indicator();
	float changeSpeed = 1.2f;
	glm::vec3 current = indicator->position();
	glm::vec3 limit = transform->position() - (0.5f * transform->up()) * button->factor();
	glm::vec3 increment = indicator->up() * changeSpeed * deltatime * button->factor();

	if ((current.y <= limit.y && limit.y < current.y + increment.y) || (current.y >= limit.y && limit.y > current.y + increment.y))
		indicator->SetPosition(limit);
	else
		indicator->MoveBy(increment);

	if (button->factor() > 0)
		button->Active((2.5f - (glm::distance(limit, indicator->position()))) / 2.5f);
	else
		button->Active(glm::distance(limit, indicator->position()) / 2.5f);
}

ButtonSystem::ButtonSystem() : System()
{
	this->AddDependency<TransformComponent>();
	this->AddDependency<MeshComponent>();
	this->AddDependency<ButtonComponent>();
	this->AddDependency<InputComponent>();
	this->AddDependency<SphereColliderComponent>();
}

void ButtonSystem::LoadSystem()
{
}

void ButtonSystem::Update(float deltatime, float time)
{
	TransformComponent* camera = Engine::GetCamera()->GetComponent<TransformComponent>();

	for (auto &entity : this->_entities)
	{
		auto input = entity->GetComponent<InputComponent>();
		auto collider = entity->GetComponent<SphereColliderComponent>();
		auto transform = entity->GetComponent<TransformComponent>();

		if (collider->Collide(camera->position(), camera->rotation(), 0.1f) &&
			input->IsMousePressed(MOUSE::LEFT_CLICK))
		{
			OnClick(entity->GetComponent<ButtonComponent>(), transform, deltatime);
		}
	}
}