#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW\glfw3.h>
#include <string>

class Display
{
private:
	const GLFWvidmode* _mode;
	GLFWwindow* _window;
	unsigned int _height;
	unsigned int _width;
	std::string _title;

	static void OnResize(GLFWwindow* window, int w, int h);

public:
	explicit Display(std::string title, unsigned int width, unsigned int height);
	explicit Display(std::string title);
	~Display();

	void BindContext();

	GLFWwindow* GetWindow();
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;

	unsigned int GetMaxHeight() const;
	unsigned int GetMaxWidth() const;

	void Resize(unsigned int width, unsigned int height);
};

#endif // !