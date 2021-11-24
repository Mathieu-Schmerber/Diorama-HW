#include "Engine.h"
#include "LevitationSystem.h"

LevitationSystem::LevitationSystem() : System()
{
	this->AddDependency<TransformComponent>();
	this->AddDependency<LevitationComponent>();
}

void LevitationSystem::Levitate(TransformComponent* transform, LevitationComponent* levitation, float time)
{
	transform->MoveBy(transform->up() * glm::sin(time * levitation->frequence() + levitation->randomStart()) * ((levitation->intensity() * _gravityScale)/10.0f));
}

void LevitationSystem::LoadSystem()
{
	LevitationComponent* levitation;

	for (auto &entity : this->_entities)
	{
		levitation = entity->GetComponent<LevitationComponent>();
		levitation->SetRandomStart(static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / 100)));
	}
}

void LevitationSystem::Update(float deltatime, float time)
{
	for (auto& entity : this->_entities)
		Levitate(entity->GetComponent<TransformComponent>(), entity->GetComponent<LevitationComponent>(), time);
}

float LevitationSystem::gravity() const
{
	return this->_gravityScale;
}

void LevitationSystem::SetGravity(float ratio)
{
	this->_gravityScale = ratio;
}

void LevitationSystem::ChangeGravitySetting(float ratio)
{
	Engine::GetLevitationSystem()->SetGravity(MAX_GRAVITY * ratio);
}

float LevitationSystem::GetGravityRatio()
{
	return Engine::GetLevitationSystem()->gravity() / MAX_GRAVITY;
}
