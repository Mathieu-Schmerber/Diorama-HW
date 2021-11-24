#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include "OBJLoader.h"
#include "TransformComponent.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal, const glm::vec4 &color)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
		this->color = color;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }
	glm::vec4* GetColor() { return &color; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec4 color;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	COLOR_VB,
	INDEX_VB
};

class MeshComponent : public Component
{
public:
	explicit MeshComponent() : Component() {}
	explicit MeshComponent(const std::string &file, bool castShadow = true);

	~MeshComponent();
	
	void Draw(TransformComponent* transform, const GLuint &shader);

	bool castShadow() { return this->_castShadow; }

protected:
private:

	bool _castShadow;
	static const unsigned int NUM_BUFFERS = 5;

	std::string _filename;

	void InitMesh(const IndexedModel& model);
	void LoadModel(const std::string& filename);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;

};