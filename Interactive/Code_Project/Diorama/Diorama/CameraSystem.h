#pragma once

#ifndef CAMERA_SYTEM
#define CAMERA_SYSTEM

#include "System.h"
#include "ShaderProgram.h"
#include "TransformComponent.h"

class CameraSystem : public System
{
public:
	explicit CameraSystem();

	void LoadSystem() override;
	void Update(float deltatime, float time) override;

private:
	void CameraView(Entity* entity);
	void CameraControl(Entity* entity, float deltatime);
};

#endif // !CAMERA_SYTEM