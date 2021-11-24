#include "RenderSystem.h"
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Core.h"
#include "WindComponent.h"

RenderSystem::RenderSystem(DirectionalLight *light) : _light(light), System()
{
	this->_defaultShader = new ShaderProgram("vs_model.glsl", "fs_model.glsl");
	this->_depthShader = new ShaderProgram("vs_shadow.glsl", "fs_shadow.glsl");
	this->_skyboxShader = new ShaderProgram("vs_skybox.glsl", "fs_skybox.glsl");
	this->_framebufferShader = new ShaderProgram("vs_framebuffer.glsl", "fs_framebuffer.glsl");
	this->_shadowMap = new ShadowMap(2024 * 4, 2024 * 4, _depthShader->GetId());
	this->_skybox = new Skybox();
	this->_postprocessing = new Framebuffer();

	this->AddDependency<TransformComponent>();
	this->AddDependency<MeshComponent>();
}

RenderSystem::~RenderSystem()
{
	delete this->_defaultShader;
	delete this->_depthShader;
	delete this->_skyboxShader;
	delete this->_framebufferShader;
	delete this->_skybox;
	delete this->_postprocessing;
}

void RenderSystem::LoadSystem()
{
	Display* display = Core::Instance().GetDisplay();
	TransformComponent* light = _light->GetComponent<TransformComponent>();

	_skyboxShader->Bind();
	glUniform1i(glGetUniformLocation(_skyboxShader->GetId(), "skybox"), 0);

	this->_skybox->Load();
	this->_shadowMap->Load();

	this->_defaultShader->Bind();
	glUniform1i(glGetUniformLocation(this->_defaultShader->GetId(), "shadow_map"), 0);

	this->_framebufferShader->Bind();
	glUniform1i(glGetUniformLocation(this->_framebufferShader->GetId(), "screen_texture"), 0);
	glUniform1i(glGetUniformLocation(this->_framebufferShader->GetId(), "bloom_texture"), 1);

	try
	{
		this->_postprocessing->Load(display->GetMaxWidth(), display->GetMaxHeight());
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

void RenderSystem::RenderMeshes(const GLuint &shader)
{
	for (auto& entity : this->_entities)
	{
		auto wind = entity->GetComponent<WindComponent>();
		auto transform = entity->GetComponent<TransformComponent>();
		auto mesh = entity->GetComponent<MeshComponent>();

		if (wind != nullptr)
		{
			glm::vec3 wind_intensity = wind->direction() * _windFactor;
			glUniform3fv(glGetUniformLocation(this->_defaultShader->GetId(), "wind_dir"), 1, &wind_intensity[0]);
		}
		else
		{
			glm::vec3 zero = glm::vec3(0, 0, 0);
			glUniform3fv(glGetUniformLocation(this->_defaultShader->GetId(), "wind_dir"), 1, &zero[0]);
		}

		if ((shader == this->_depthShader->GetId() && mesh->castShadow()) || shader != this->_depthShader->GetId())
			mesh->Draw(transform, shader);
	}
}

void RenderSystem::RenderDefaultBuffer(Display *display)
{
	TransformComponent* camera = Engine::GetCamera()->GetComponent<TransformComponent>();
	TransformComponent* lightTransform = _light->GetComponent<TransformComponent>();

	glViewport(0, 0, display->GetWidth(), display->GetHeight());

	this->_defaultShader->Bind();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)display->GetWidth() / (float)display->GetHeight(), 0.1f, 100.0f);
	glm::mat4 view = camera->viewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(this->_defaultShader->GetId(), "proj_matrix"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->_defaultShader->GetId(), "view_matrix"), 1, GL_FALSE, &view[0][0]);
	glUniform3fv(glGetUniformLocation(this->_defaultShader->GetId(), "light_pos"), 1, &lightTransform->position()[0]);
	glUniformMatrix4fv(glGetUniformLocation(this->_defaultShader->GetId(), "camera_matrix"), 1, GL_FALSE, &camera->matrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(this->_defaultShader->GetId(), "light_space_matrix"), 1, GL_FALSE, &_shadowMap->lightSpaceMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _shadowMap->depthMap());
	this->RenderMeshes(this->_defaultShader->GetId());
}

void RenderSystem::RenderShadowBuffer(Display* display)
{
	TransformComponent* lightTransform = _light->GetComponent<TransformComponent>();

	this->_shadowMap->Process(lightTransform);
	this->RenderMeshes(_depthShader->GetId());
	glBindFramebuffer(GL_FRAMEBUFFER, _postprocessing->fbo());;
}

void RenderSystem::Update(float deltatime, float time)
{
	Display* display = Core::Instance().GetDisplay();

	// Bind the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _postprocessing->fbo());
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	_defaultShader->Bind();
	glUniform1f(glGetUniformLocation(_defaultShader->GetId(), "time"), time);

	_framebufferShader->Bind();
	glUniform1f(glGetUniformLocation(_framebufferShader->GetId(), "exposure"), _exposure);

	// Shadow mapping
	this->RenderShadowBuffer(display);

	// Default mapping
	this->RenderDefaultBuffer(display);

	// Skybox
	this->_skybox->Draw(this->_skyboxShader->GetId(), display->GetWidth(), display->GetHeight());

	// Post processing
	this->_postprocessing->Draw(_framebufferShader->GetId(), display->GetWidth(), display->GetHeight());
	glfwSwapBuffers(display->GetWindow());
}

float RenderSystem::exposure() const
{
	return this->_exposure;
}

void RenderSystem::SetExposure(float exposure)
{
	this->_exposure = exposure;
}

void RenderSystem::ChangeExposureSetting(float ratio)
{
	Engine::GetRenderSystem()->SetExposure(MAX_EXPOSURE * ratio);
}

float RenderSystem::GetExposureRatio()
{
	return MAX_EXPOSURE / Engine::GetRenderSystem()->exposure();
}

float RenderSystem::wind() const
{
	return this->_windFactor;
}

void RenderSystem::SetWind(float ratio)
{
	this->_windFactor = ratio;
}

void RenderSystem::ChangeWindSetting(float ratio)
{
	Engine::GetRenderSystem()->SetWind(MAX_WIND * ratio);
}

float RenderSystem::GetWindRatio()
{
	return MAX_WIND / Engine::GetRenderSystem()->wind();
}
