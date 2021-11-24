#include "ShadowMap.h"
#include "ShaderProgram.h"

ShadowMap::ShadowMap(unsigned int width, unsigned int height, const GLuint &shader) : _width(width), _height(height), _shader(shader) {}

ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &_depthMapFBO);
	glDeleteTextures(1, &_depthMap);
}

void ShadowMap::Load()
{
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Shadow smoothing
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->_depthMap = depthMap;
	this->_depthMapFBO = depthMapFBO;
}

void ShadowMap::Process(TransformComponent* lightTransform)
{
	glm::mat4 lightProjection, lightView;
	float near_plane = 0.1f, far_plane = 75.0f;

	lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightTransform->position(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	_lightSpaceMatrix = lightProjection * lightView;
	ShaderProgram::Bind(_shader);
	glUniformMatrix4fv(glGetUniformLocation(_shader, "light_space_matrix"), 1, GL_FALSE, &_lightSpaceMatrix[0][0]);

	glViewport(0, 0, _width, _height);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
}
