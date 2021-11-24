#pragma once

#ifndef PATH_COMPONENT_H
#define PATH_COMPONENT_H

#include <vector>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include "Component.h"

class PathComponent : public Component
{
private:
	unsigned int _waypointIndex = 0;
	std::vector<glm::vec3> _waypoints;

public:
	glm::vec3 _targetDir;

	explicit PathComponent() : Component() {}

	void SetWaypoints(const std::vector<glm::vec3>& waypoints) { this->_waypoints = waypoints; };

	glm::vec3 GetCurrentWaypoint() const { return this->_waypoints[this->_waypointIndex]; };

	glm::vec3 SelectWaypoint(unsigned int index)
	{
		this->_waypointIndex = index;
		if (this->_waypointIndex >= this->_waypoints.size())
			this->_waypointIndex = 0;
		return GetCurrentWaypoint();
	}

	glm::vec3 SelectNextWaypoint()
	{ 
		this->_waypointIndex++;
		if (this->_waypointIndex >= this->_waypoints.size())
			this->_waypointIndex = 0;
		return GetCurrentWaypoint();
	};

	bool WaypointReached(glm::vec3 position, float tolerance)
	{
		return (glm::distance(position, GetCurrentWaypoint()) <= tolerance);
	};
};

#endif // !PATH_COMPONENT_H