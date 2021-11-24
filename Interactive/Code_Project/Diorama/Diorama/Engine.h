#pragma once
#ifndef ENGINE_H

#include <vector>
#include <memory>
#include "System.h"
#include "RenderSystem.h" // include glew
#include "LevitationSystem.h" // include glew
#include "Camera.h" // include glfw
#include "DirectionalLight.h" // include glfw

class Engine
{
	Camera* _camera;
	DirectionalLight* _directionalLight;
	RenderSystem* _renderSystem;
	LevitationSystem* _levitationSystem;
	std::vector<System *> _systems;
	std::vector<Entity *> _entities;

	GLfloat _currentTime;
	GLfloat _deltatime;
	GLfloat _lastTime;

public:

	Engine();
	~Engine();

	void Run();

	Camera* camera();
	static Camera* GetCamera();

	DirectionalLight* directionalLight();
	static DirectionalLight* GetDirectionalLight();

	RenderSystem* renderSystem();
	static RenderSystem* GetRenderSystem();

	LevitationSystem* levitationSystem();
	static LevitationSystem* GetLevitationSystem();

private:
	void Load();
	void LoadSystems();
	void RegisterEntity(Entity* entity);
	void RegisterEntity(std::vector<Entity*> entities);
};

#endif // !ENGINE_H