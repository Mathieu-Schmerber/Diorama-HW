#include "MeshComponent.h"
#include "Core.h"
#include "Exception.h"

Core* Core::_instance;

void Core::InitGlew()
{
	glewExperimental = GL_TRUE;
	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW)
		throw new Exception("GLEW", "Error while loading glew.");
}

void Core::InitGlfw()
{
	if (!glfwInit())
		throw new Exception("GLFW", "Error while loading glfw.");
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	this->_display = new Display("Diorama");
	this->_display->BindContext();
}

Core::Core()
{
	_instance = this;
}

Core::~Core()
{
	delete this->_display;
	delete this->_engine;
}

void Core::Start()
{
	try
	{
		this->InitGlfw();
		this->InitGlew();

		glfwSwapInterval(1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 2);
		glfwWindowHint(GLFW_STEREO, GL_FALSE);

		this->LoadResources();

		this->_running = true;
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

void Core::LoadResources()
{
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	try
	{
		this->_engine = new Engine();
	}
	catch (const std::exception&ex)
	{
		throw ex;
	}
}

void Core::Update()
{
	try
	{
		glfwPollEvents();

		this->_engine->Run();
		this->_running &= (glfwGetKey(_display->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		this->_running &= (glfwWindowShouldClose(_display->GetWindow()) != GL_TRUE);
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
}

bool Core::IsRunning()
{
	return _running;
}

Display* Core::GetDisplay()
{
	return this->_display;
}

Engine* Core::GetEngine()
{
	return this->_engine;
}