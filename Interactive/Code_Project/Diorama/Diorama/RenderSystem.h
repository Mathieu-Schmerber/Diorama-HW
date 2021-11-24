#pragma once

#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Skybox.h"
#include "Framebuffer.h"
#include "ShadowMap.h"
#include "ShaderProgram.h"
#include "System.h"
#include "DirectionalLight.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

class Display;

const float MAX_EXPOSURE = 10.0f;
const float MAX_WIND = 10.0f;

class RenderSystem : public System
{
private:
	DirectionalLight *_light;
	ShaderProgram* _defaultShader;
	ShaderProgram* _depthShader;
	ShaderProgram* _skyboxShader;
	ShaderProgram* _framebufferShader;
	ShadowMap* _shadowMap;
	Skybox* _skybox;
	Framebuffer* _postprocessing;

	float _exposure = 2.0f;
	float _windFactor = 1.0f;

	void RenderMeshes(const GLuint& shader);
	void RenderShadowBuffer(Display* display);
	void RenderDefaultBuffer(Display* display);

public:

	explicit RenderSystem(DirectionalLight *light);

	~RenderSystem();

	void LoadSystem() override;
	void Update(float deltatime, float time) override;

	float exposure() const;
	void SetExposure(float ratio);
	static void ChangeExposureSetting(float ratio);
	static float GetExposureRatio();

	float wind() const;
	void SetWind(float ratio);
	static void ChangeWindSetting(float ratio);
	static float GetWindRatio();
};

#endif // !RENDER_SYSTEM_H