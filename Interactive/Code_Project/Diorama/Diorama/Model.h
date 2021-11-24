#pragma once

#ifndef MODEL_H
#define MODEL_H

#include "Entity.h"
#include "TransformComponent.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include "MeshComponent.h"

class Model : public Entity
{
public:
	explicit Model(const std::string& filename, const glm::vec3& position = {0, 0, 0}, bool castShadow = true) : Entity()
	{
		this->AddComponent<TransformComponent>(position);
		this->AddComponent<MeshComponent>(filename, castShadow);
	}
};

#endif // !MODEL_H