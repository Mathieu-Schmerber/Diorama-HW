#pragma once

#ifndef SWAN_H
#define SWAN_H

#include "Entity.h"
#include "MeshComponent.h"
#include "PathComponent.h"
#include "VelocityComponent.h"

class Swan : public Entity
{
public:
	explicit Swan(const std::string &model, unsigned int index) : Entity()
	{
		PathComponent *path = this->AddComponent<PathComponent>();

		path->SetWaypoints({
			{0.024, 9.7, 6.807},
			{3.854, 9.5, 4.716},
			{2.966, 9.7, 1.640},
			{-0.491, 9.7, 2.770}
		});
		path->SelectWaypoint(index);

		this->AddComponent<MeshComponent>(model);
		this->AddComponent<VelocityComponent>(0.5f);
		this->AddComponent<TransformComponent>(path->GetCurrentWaypoint());
	}
};


#endif // !SWAN_H