#pragma once

#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include <GL\glew.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "TransformComponent.h"

class ShadowMap
{
private:
	unsigned int _depthMapFBO;
	unsigned int _depthMap;
	unsigned int _width;
	unsigned int _height;
	GLuint _shader;
	glm::mat4 _lightSpaceMatrix;

public:
	explicit ShadowMap(unsigned int width, unsigned int height, const GLuint& shader);
	~ShadowMap();

	void Load();
	void Process(TransformComponent* lightTransform);

	unsigned int depthMap() const { return this->_depthMap; }
	glm::mat4 lightSpaceMatrix() const { return this->_lightSpaceMatrix; }
};

#endif // !SHADOW_MAP_H