#pragma once

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL\glew.h>

class Framebuffer
{
private:
	GLuint _texture;
	GLuint _VAO;
	GLuint _FBO;

public:
	explicit Framebuffer() = default;
	~Framebuffer();

	void Load(unsigned int width, unsigned int height);
	void Draw(const GLuint &shader, int width, int height);

	GLuint texture() const { return this->_texture; };
	GLuint vao() const { return this->_VAO; };
	GLuint fbo() const { return this->_FBO; };
};

#endif // !FRAMEBUFFER_H