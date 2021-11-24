#pragma once

#ifndef BUTTON_SYSTEM_H
#define BUTTON_SYSTEM_H

#include "System.h"
#include "ButtonComponent.h"

class ButtonSystem : public System
{
private:
	void OnClick(ButtonComponent* button, TransformComponent *transform, float deltatime);

public:
	explicit ButtonSystem();

	void LoadSystem() override;
	void Update(float deltatime, float time) override;
};

#endif // !BUTTON_SYSTEM_H