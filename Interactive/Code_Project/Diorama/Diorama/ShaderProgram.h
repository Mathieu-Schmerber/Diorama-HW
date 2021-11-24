#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

class ShaderProgram
{
private:
	GLuint _id;

	std::string ReadShader(const char* filename);
	void CheckError(GLuint shader);

public:
	ShaderProgram(const char* vertex, const char *fragment);
	~ShaderProgram();

	void Bind();
	static void Bind(GLuint id);

	GLuint GetId();
};

#endif // !SHADER_H