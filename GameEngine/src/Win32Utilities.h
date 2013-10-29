#pragma once
#include "GraphicsContext.h"

class Win32Utilities
{
public:
  static GraphicsContext* graphics;
  static int returnCode;
  static bool createWindow(char * title, int width, int height, GraphicsContext* graphics);
  static bool Update();
};