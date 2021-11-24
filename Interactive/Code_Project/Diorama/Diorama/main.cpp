#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL

#include "Core.h"

int main()
{
	try
	{
		Core* core = new Core();

		core->Start();
		while (core->IsRunning())
			core->Update();
		delete core;
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	glfwTerminate();
	return 0;
}