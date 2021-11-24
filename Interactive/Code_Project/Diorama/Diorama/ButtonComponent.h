#pragma once

#ifndef BUTTON_COMPONENT_H
#define BUTTON_COMPONENT_H

#include "TransformComponent.h"

class ButtonComponent : public Component
{
private:
	TransformComponent* _indicator;
	float _increaseFactor;
	void (*_onclick)(float);

public:

	// Setting limit in world pos

	explicit ButtonComponent(TransformComponent *indicator = nullptr, 
		float increaseFactor = 1, void (*onclick)(float) = nullptr)
		: _indicator(indicator), _increaseFactor(increaseFactor), _onclick(onclick), Component() {}

	TransformComponent* indicator() const { return this->_indicator; }

	float factor() const { return _increaseFactor; }
	void Active(float value) { this->_onclick(value); }
};

#endif // !BUTTON_COMPONENT_H