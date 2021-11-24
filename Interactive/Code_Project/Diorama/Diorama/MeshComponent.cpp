#include "MeshComponent.h"
//#include "util.h"
//#include "debugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "Engine.h"

void MeshComponent::LoadModel(const std::string& filename) {
	try
	{
		InitMesh(OBJModel(filename).ToIndexedModel());
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

void MeshComponent::InitMesh(const IndexedModel& model)
{
	m_numIndices = (unsigned int) model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.colors[0]) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

MeshComponent::MeshComponent(const std::string &filename, bool castShadow) : _filename(filename), _castShadow(castShadow), Component()
{
	LoadModel(filename);
}

MeshComponent::~MeshComponent()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void MeshComponent::Draw(TransformComponent* transform, const GLuint &shader)
{
	Camera* cam = Engine::GetCamera();
	TransformComponent* light = Engine::GetDirectionalLight()->GetComponent<TransformComponent>();
	CameraComponent* camera = cam->GetComponent<CameraComponent>();
	TransformComponent* camTransform = cam->GetComponent<TransformComponent>();
	glm::mat4 viewMatrix = camera->viewMatrix();
	glm::mat4 proj_matrix = camera->projectionMatrix();
	glm::mat4 modelMatrix = transform->matrix();

	ShaderProgram::Bind(shader);
	//glUniformMatrix4fv(glGetUniformLocation(shader, "camera_matrix"), 1, GL_FALSE, &camTransform->matrix()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model_matrix"), 1, GL_FALSE, &modelMatrix[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shader, "view_matrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(shader, "proj_matrix"), 1, GL_FALSE, &proj_matrix[0][0]);

	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}