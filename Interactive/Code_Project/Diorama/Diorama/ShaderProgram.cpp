#include "ShaderProgram.h"

std::string ShaderProgram::ReadShader(const char* filename)
{
	std::string vs_text;
	std::ifstream vs_file(filename);

	std::string vs_line;
	if (vs_file.is_open())
	{
		while (getline(vs_file, vs_line))
		{
			vs_text += vs_line;
			vs_text += '\n';
		}
		vs_file.close();
	}
	return vs_text;
}

void ShaderProgram::CheckError(GLuint shader)
{
	// Get log lenght
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1)
	{
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		std::cout << "--------------Shader compilation error-------------\n";
		std::cout << errorLog.data();
	}
}

ShaderProgram::ShaderProgram(const char* vertex, const char* fragment)
{
	std::string vertexCode = ReadShader(vertex);
	std::string fragmentCode = ReadShader(fragment);
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	this->_id = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CheckError(vertexShader);
	glAttachShader(this->_id, vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CheckError(fragmentShader);
	glAttachShader(this->_id, fragmentShader);

	glLinkProgram(this->_id);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::Bind()
{
	glUseProgram(_id);
}

void ShaderProgram::Bind(GLuint id)
{
	glUseProgram(id);
}

GLuint ShaderProgram::GetId()
{
	return this->_id;
}
