#pragma once

#ifndef VELOCITY_COMPONENT_H
#define VELOCITY_COMPONENT_H

#include "Component.h"

class VelocityComponent : public Component
{
protected:
	float _velocity;
public:
	explicit VelocityComponent() : Component() {}
	explicit VelocityComponent(float velocity) : _velocity(velocity), Component() {}

	float GetSpeed() const { return this->_velocity; }
};


#endif // !VELOCITY_COMPONENT_H