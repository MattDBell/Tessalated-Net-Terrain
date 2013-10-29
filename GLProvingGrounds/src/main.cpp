#include "GraphicsContext.h"
#include "GraphicsComponent.h"
#include <ctime>
#include "Input.h"
#include "TestScene.h"

#ifdef _WIN32
#include "Win32Utilities.h"
typedef Win32Utilities OSUTILITIES;
#endif

//Test CPP
GraphicsContext graphics;
bool running = true;
#include "Debugging.h"

int WINAPI WinMain (HINSTANCE, HINSTANCE,
					LPSTR, int){
	TestScene*  scene = NULL;


	OSUTILITIES::createWindow("OpenGL Stuff", 780, 780, &graphics);

	graphics.ResizeWindow(780, 780);
	
	scene = new TestScene(&graphics);
	std::clock_t beginning;
	std::clock_t end;
	float deltaTime = 0.0f;
	LOG("main.log", "Entering Main Loop");
	while (running){
		beginning = std::clock();

    running = OSUTILITIES::Update();
		scene->Update(deltaTime);
		graphics.RenderAll();
		//graphics.RenderPass(0, NULL, scene->cam);
		end = std::clock();
		deltaTime = (float)(end- beginning) / CLOCKS_PER_SEC;
	}
	Debugging::Flush();
	delete scene;
	return OSUTILITIES::returnCode;
}