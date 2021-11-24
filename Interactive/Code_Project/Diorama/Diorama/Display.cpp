#include "Core.h"
#include "Exception.h"

void Display::OnResize(GLFWwindow* window, int w, int h)
{
	if (w > 0 && h > 0)
	{
		Display* display = Core::Instance().GetDisplay();

		display->Resize(w, h);
	}
}

Display::Display(std::string title, unsigned int width, unsigned int height) : _height(height), _width(width), _title(title)
{
	_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
	glfwSetWindowSizeCallback(_window, Display::OnResize);

	if (!_window)
		throw new Exception("Window", "Error while creating the window");
}

Display::Display(std::string title)
{
	_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	_window = glfwCreateWindow(_mode->width, _mode->height, _title.c_str(), NULL, NULL);
	this->_height = _mode->height;
	this->_width = _mode->width;

	glfwSetWindowMonitor(_window, NULL, 0, 0, this->_width, this->_height, 60);
	glfwSetWindowSizeCallback(_window, Display::OnResize);

	if (!_window)
		throw new Exception("Window", "Error while creating the window");
}

Display::~Display()
{
	glfwMakeContextCurrent(this->_window);
	glfwTerminate();
}

void Display::BindContext()
{
	glfwMakeContextCurrent(_window);
}

GLFWwindow* Display::GetWindow()
{
	return this->_window;
}

unsigned int Display::GetHeight() const
{
	return this->_height;
}

unsigned int Display::GetWidth() const
{
	return this->_width;
}

unsigned int Display::GetMaxHeight() const
{
	return this->_mode->height;
}

unsigned int Display::GetMaxWidth() const
{
	return this->_mode->width;
}

void Display::Resize(unsigned int width, unsigned int height)
{
	this->_width = width;
	this->_height = height;
}
