#ifndef GRAPHICSENGINE
#define GRAPHICSENGINE

#include <Windows.h>
#include <vector>
#include <GL/glew.h>
#include <GL/wglew.h>
#include "Texture.h"





class GraphicsComponent;
class Camera;
class ShaderProgram;

class GraphicsContext
{
public:
	GraphicsContext();
	GraphicsContext(HWND hwnd);
	~GraphicsContext();

	bool			CreateContext(HWND hwnd);
	void			ResizeWindow(int w, int h);
	void			RenderPass(int pass, ShaderProgram* defaultShader, Camera* cam);
	void			RenderAll();
	void			RegisterGraphComp(GraphicsComponent* toAdd, int pass);
	void			RegisterCamera(Camera* toAdd, int pass);
	void			RegisterShader(ShaderProgram* toAdd, int pass);
	int				GetWindowWidth();
	int				GetWindowHeight();
private:
	int numPasses;
	ShaderProgram* defaultShaderForPass;
	std::vector<Camera*>							cameraPerPass;
	std::vector<ShaderProgram*>						defShaderPerPass;
	std::vector<std::vector<GraphicsComponent*>>	entitiesPerPass;
	int windowWidth;
	int windowHeight;
	
protected:
	HGLRC hrc;
	HDC hdc;
	HWND hwnd;
};

#endif