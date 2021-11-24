#pragma once

#ifndef LEVITATION_SYTEM
#define LEVITATION_SYTEM

#include "System.h"
#include "TransformComponent.h"
#include "LevitationComponent.h"

const float MAX_GRAVITY = 10.0f;

class LevitationSystem : public System
{
private:
	float _gravityScale = 1.0f;
	
	void Levitate(TransformComponent* transform, LevitationComponent* levitation, float time);

public:

	explicit LevitationSystem();

	void LoadSystem() override;
	void Update(float deltatime, float time) override;

	float gravity() const;
	void SetGravity(float ratio);
	static void ChangeGravitySetting(float ratio);
	static float GetGravityRatio();
};

#endif // !LEVITATION_SYTEM