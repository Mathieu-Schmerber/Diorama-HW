#pragma once

#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL\glew.h>

class Skybox
{
private:
	GLuint _vao;
	unsigned int _texture;

public:
	explicit Skybox() = default;
	~Skybox();

	void Load();
	void Draw(const GLuint& shader, unsigned int width, unsigned int height);
};

#endif // !SKYBOX_H