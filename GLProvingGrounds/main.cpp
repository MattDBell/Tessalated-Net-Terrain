#include "GraphicsContext.h"
#include "GraphicsComponent.h"
#include <ctime>
#include "Input.h"
#include "TestScene.h"
//Test CPP
GraphicsContext graphics;
bool running = true;

HINSTANCE hInstance;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch(message){
	case WM_SIZE:
		graphics.ResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		Input::Get()->UpdateWith(wParam, true);
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		Input::Get()->UpdateWith(wParam, false);
		break;
	case WM_KILLFOCUS:
		Input::Get()->LoseFocus();
		break;
	case WM_SETFOCUS:
		Input::Get()->GainFocus();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool createWindow(LPCWSTR title, int width, int height){
	WNDCLASS windowClass;
	HWND hWnd;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	hInstance = GetModuleHandle(NULL);

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC) WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = title;

	if(!RegisterClass(&windowClass)){
		return false;
	}

	hWnd = CreateWindowEx(dwExStyle, title, title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

	graphics.CreateContext(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					LPSTR lpCmdLine, int nCmdShow){
	MSG msg;
	TestScene*  scene = NULL;
	char *orig = "OpenGL Proving Grounds";
	size_t origsize = strlen(orig) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig, _TRUNCATE);

	createWindow(wcstring, 500, 500);

	graphics.ResizeWindow(500, 500);
	scene = new TestScene(&graphics);
	std::clock_t beginning;
	std::clock_t end;
	float deltaTime = 0.0f;
	while (running){
		beginning = std::clock();
		while(PeekMessage(&msg, NULL, 0,0, PM_REMOVE)){
			switch(msg.message){
			case WM_QUIT:
				running = false;
				break;
			default:
				TranslateMessage(&msg);
				graphics.RenderAll();
				DispatchMessage(&msg);
			}
		}
		//graphics.RenderPass(0, NULL, scene->cam);
		end = std::clock();
		deltaTime = (float)(end- beginning) / CLOCKS_PER_SEC;
	}
	delete scene;
	return (int) msg.wParam;
}