#pragma once

#ifndef LEVITATION_COMPONENT_H
#define LEVITATION_COMPONENT_H

#include "Component.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<glm/gtx/string_cast.hpp>

class LevitationComponent : public Component
{
private:
	float _frequence;
	float _intensity;
	float _randomStart;

public:
	explicit LevitationComponent(const float& frequence = 1, const float&intensity = 1) 
		: _frequence(frequence), _intensity(intensity), Component() {}

	float intensity() const { return this->_intensity; }
	float frequence() const { return this->_frequence; }
	float randomStart() const { return this->_randomStart; }
	void SetRandomStart(float value) { _randomStart = value; }
};

#endif // !LEVITATION_COMPONENT_H