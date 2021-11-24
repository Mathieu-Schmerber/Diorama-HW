#include "WaypointSystem.h"
#include "TransformComponent.h"
#include "VelocityComponent.h"
#include "PathComponent.h"

WaypointSystem::WaypointSystem()
{
	this->AddDependency<TransformComponent>();
	this->AddDependency<VelocityComponent>();
	this->AddDependency<PathComponent>();
}

void WaypointSystem::MoveEntity(Entity* entity, float deltatime, float time)
{
	const float turnDistance = 1.0f;
	const float turnSpeed = 0.4f;
	PathComponent* path = entity->GetComponent<PathComponent>();
	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	VelocityComponent* velocity = entity->GetComponent<VelocityComponent>();
	glm::vec3 destination = path->GetCurrentWaypoint();

	if (path->WaypointReached(transform->position(), turnDistance))
		destination = path->SelectNextWaypoint();
	transform->SetForward(glm::mix(transform->forward(), glm::normalize(destination - transform->position()), turnSpeed * deltatime));
	transform->MoveBy(transform->forward() * velocity->GetSpeed() * deltatime);

	// Adding a bit of floating movement on y
	transform->SetForward(glm::vec3(transform->forward().x, transform->forward().y + glm::sin(time * 2) * 0.002f, transform->forward().z));
}

void WaypointSystem::LoadSystem()
{
}

void WaypointSystem::Update(float deltatime, float time)
{
	for (auto& entity : this->_entities)
		MoveEntity(entity, deltatime, time);
}
