#include <Windows.h>
#include <Windowsx.h>

#include "Input.h"
#include "Win32Utilities.h"



HINSTANCE hInstance;
GraphicsContext * Win32Utilities::graphics = 0;
int Win32Utilities::returnCode = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

  switch(message){
  case WM_SIZE:
    if(Win32Utilities::graphics)
      Win32Utilities::graphics->ResizeWindow(LOWORD(lParam), HIWORD(lParam));
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
    {
      Input::Get()->GainFocus();
      PBYTE arr = (PBYTE)malloc(sizeof(char) * 256);
      if(GetKeyboardState(arr)){
        for(size_t i = 0; i < 256; ++i)
          Input::Get()->UpdateWith(i, (arr[i] >> 7 > 0));//   keyStates[i] = (arr[i]>> 7 )>0;
      }
    }
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_LBUTTONDOWN:
    Input::Get()->LMouseUpdate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
    break;
  case WM_LBUTTONUP:
    Input::Get()->LMouseUpdate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
  case WM_MOUSEMOVE:
    Input::Get()->MousePosUpdate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);
}


bool Win32Utilities::createWindow(char* orig, int width, int height, GraphicsContext* graphics){
  
  size_t origsize = strlen(orig) + 1;
  const size_t newsize = 100;
  size_t convertedChars = 0;
  wchar_t wcstring[newsize];
  mbstowcs_s(&convertedChars, wcstring, origsize, orig, _TRUNCATE);


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
  windowClass.lpszClassName = wcstring;

  if(!RegisterClass(&windowClass)){
    return false;
  }

  hWnd = CreateWindowEx(dwExStyle, wcstring, wcstring, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

  graphics->CreateContext(hWnd);

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  Win32Utilities::graphics = graphics;
  return true;
}

bool Win32Utilities::Update()
{
  MSG msg;
  while(PeekMessage(&msg, NULL, 0,0, PM_REMOVE)){
    switch(msg.message){
    case WM_QUIT:
      returnCode = (int) msg.wParam;
      return false;
    default:
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  
  return true;
}