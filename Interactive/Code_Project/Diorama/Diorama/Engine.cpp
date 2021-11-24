#include "CameraSystem.h"
#include "WaypointSystem.h"
#include "LevitationSystem.h"
#include "ButtonSystem.h"
#include "Model.h"
#include "SettingsKit.h"
#include "WindActor.h"
#include "LevitatingActor.h"
#include "Swan.h"
#include "Core.h"
#include "Engine.h"
#include "WindComponent.h"

Engine::Engine()
{
	this->Load();
}

Engine::~Engine()
{
	for (auto& sys : this->_systems)
		delete sys;
	for (auto& entity : this->_entities)
		delete entity;
	this->_systems.clear();
	this->_entities.clear();
}

void Engine::RegisterEntity(Entity* entity)
{
	this->_entities.push_back(entity);
	for (auto& sys : this->_systems)
	{
		if (this->_entities.back()->HasComponents(sys->GetDependencies()))
			sys->AddEntity(this->_entities.back());
	}
}

void Engine::RegisterEntity(std::vector<Entity*> entities)
{
	for (auto& e : entities)
		RegisterEntity(e);
}

void Engine::LoadSystems()
{
	_renderSystem = new RenderSystem(_directionalLight);
	_levitationSystem = new LevitationSystem();

	this->_systems.push_back(_renderSystem);
	this->_systems.push_back(new CameraSystem());
	this->_systems.push_back(new WaypointSystem());
	this->_systems.push_back(_levitationSystem);
	this->_systems.push_back(new ButtonSystem());
}

void Engine::Load()
{
	this->_camera = new Camera({ -15.867956, 17.775911, 35.874397 }, glm::degrees(glm::vec3(0.007148, -0.003247, -0.015463)));
	this->_directionalLight = new DirectionalLight({ 15, 30, 15 });

	this->LoadSystems();

	SettingsKit exposureSetting({ 8, 10, 10 }, _renderSystem->exposure() / MAX_EXPOSURE, RenderSystem::ChangeExposureSetting, "LightTxt.obj");
	SettingsKit windSetting({ 10, 10, 10 }, _renderSystem->wind() / MAX_WIND, RenderSystem::ChangeWindSetting, "WindTxt.obj");
	SettingsKit gravitySetting({ 12, 10, 10 }, _levitationSystem->gravity() / MAX_GRAVITY, LevitationSystem::ChangeGravitySetting, "GravityTxt.obj");

	this->RegisterEntity(this->_camera);
	this->RegisterEntity(this->_directionalLight);
	this->RegisterEntity(new Model("Island.obj"));
	this->RegisterEntity(new Model("Willow.obj"));
	this->RegisterEntity(new WindActor("Water.obj", { 0, 1, 0 }, {0, 0, 0}, false));
	this->RegisterEntity(new WindActor("Grass.obj", { 0.3, 0.1, 0 }));
	this->RegisterEntity(new WindActor("WaterPlants.obj", { 0.2, 0.5, 0 }));
	this->RegisterEntity(new WindActor("Folliages.obj", { 1, 0, 0 }));
	this->RegisterEntity(new WindActor("WillowFolliages.obj", { 1, 0, 0 }));
	this->RegisterEntity(new LevitatingActor("Balloon.obj", 0.5f, 0.05f));
	this->RegisterEntity(new LevitatingActor("TinyRocks.obj", 2.0f, 0.03f));
	this->RegisterEntity(new LevitatingActor("SmallRocks.obj", 1.0f, 0.03f));
	this->RegisterEntity(new LevitatingActor("HeavyRocks.obj", 0.5f, 0.03f));
	this->RegisterEntity(new Swan("Swan.obj", 0));
	this->RegisterEntity(new Swan("Swan.obj", 1));
	this->RegisterEntity(exposureSetting.GetEntities());
	this->RegisterEntity(windSetting.GetEntities());
	this->RegisterEntity(gravitySetting.GetEntities());

	// Init systems when populated by entities
	for (auto& sys : this->_systems)
		sys->LoadSystem();
}

void Engine::Run()
{
	_lastTime = _currentTime;
	_currentTime = (GLfloat)glfwGetTime();
	_deltatime = _currentTime - _lastTime;

	for (auto& sys : this->_systems)
		sys->Update(_deltatime, _currentTime);
}

Camera* Engine::camera()
{
	return this->_camera;
}

Camera *Engine::GetCamera()
{
	return Core::Instance().GetEngine()->camera();
}

DirectionalLight* Engine::directionalLight()
{
	return this->_directionalLight;
}

DirectionalLight* Engine::GetDirectionalLight()
{
	return Core::Instance().GetEngine()->directionalLight();
}

RenderSystem* Engine::renderSystem()
{
	return this->_renderSystem;
}

RenderSystem* Engine::GetRenderSystem()
{
	return Core::Instance().GetEngine()->renderSystem();
}

LevitationSystem* Engine::levitationSystem()
{
	return this->_levitationSystem;
}

LevitationSystem* Engine::GetLevitationSystem()
{
	return Core::Instance().GetEngine()->levitationSystem();;
}
