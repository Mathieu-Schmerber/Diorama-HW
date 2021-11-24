#ifndef CORE_H
#define CORE_H

#include <iostream>
#include "Engine.h"
#include "Display.h"

#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

class Core
{

private:

	static Core* _instance;

	Display *_display;
	Engine* _engine;
	bool _running;

	void InitGlfw();
	void InitGlew();
	void LoadResources();

public:

	static Core &Instance()
	{
		if (_instance == nullptr)
			_instance = new Core();
		return *_instance;
	}

	explicit Core();
	~Core();

	void Start();
	void Update();

	bool IsRunning();
	GLfloat GetDeltatime();
	Display* GetDisplay();
	Engine* GetEngine();
};

#endif