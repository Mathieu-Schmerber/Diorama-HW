#pragma once

#ifndef WAYPOINT_SYSTEM
#define WAYPOINT_SYSTEM

#include "System.h"

class WaypointSystem : public System
{
private:
	void MoveEntity(Entity* entity, float deltatime, float time);

public:

	explicit WaypointSystem();

	void LoadSystem() override;
	void Update(float deltatime, float time) override;
};

#endif // !WAYPOINT_SYSTEM