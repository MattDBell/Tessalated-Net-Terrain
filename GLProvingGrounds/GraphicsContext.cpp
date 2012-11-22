#include "GraphicsContext.h"
#include "GraphicsComponent.h"
#include "Camera.h"

GraphicsContext::GraphicsContext()	
{
}

GraphicsContext::GraphicsContext(HWND hwnd)
{
	CreateContext(hwnd);
}

GraphicsContext::~GraphicsContext(){
	wglMakeCurrent(hdc, 0);
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
}

//Attempts to create a context, returns true there is now a context.  May not be 4.0 context
bool GraphicsContext::CreateContext(HWND hwnd){
	
	this->hwnd = hwnd;
	//Get the Device Context from the Handler to the Window
	hdc = GetDC(hwnd);

	//Define out pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);

	if(nPixelFormat == 0)
		return false;

	if(!SetPixelFormat(hdc, nPixelFormat, &pfd))
		return false;
	//Create a temporary default GL Context in order to create a specific context
	HGLRC tempOpenGLContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempOpenGLContext);

	//Ensure Glew is working
	GLenum error = GLCALL(glewInit());
	if(error != GLEW_OK)
		return false;

	
	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	//Attempt to create context defined by attributes.  If succeed, delete old context and make new one
	//Current, else make pointer to context = temporary
	if(wglewIsSupported("WGL_ARB_create_context") == 1){
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
		if(!hrc){
			hrc = tempOpenGLContext;
		} else {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(tempOpenGLContext);
			wglMakeCurrent(hdc, hrc);
	//		CheckErrors();
		}

	} else {
		hrc = tempOpenGLContext;
	}
	GLCALL( glEnable(GL_CULL_FACE) );
	GLCALL( glCullFace(GL_FRONT) );
	GLCALL( glEnable(GL_DEPTH_TEST) );
	GLCALL( glDepthFunc(GL_LEQUAL) );
	//error = GLCALL(glewInit());
	//if(error != GLEW_OK)
	//	return false;



	return true;
}

void GraphicsContext::ResizeWindow(int w, int h){
	windowWidth = w;
	windowHeight = h;
}

void GraphicsContext::RenderPass(int pass, ShaderProgram* defaultShader, Camera* cam){

	GLCALL(glViewport(0,0, windowWidth, windowHeight));
	//Set RenderTarget
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	//Need a constant buffer for Camera shtuff
	if(cam != NULL)
		cam->SetCurrent();
	if(defaultShader != NULL){
		defaultShader->Bind();
		defaultShaderForPass = defaultShader;
	}
	
	for( auto it = entitiesPerPass[pass].begin(); it != entitiesPerPass[pass].end(); it++){
		(*it)->Render(pass);
	}
	
	if(pass == numPasses-1)
		SwapBuffers(hdc);
}

void GraphicsContext::RenderAll(){
	for(int i = 0; i < numPasses; ++i){
		RenderPass(i, defShaderPerPass[i], cameraPerPass[i]);
	}
}

void GraphicsContext::RegisterGraphComp(GraphicsComponent* toAdd, int pass){
	if(pass >= numPasses){
		numPasses = pass +1;
		cameraPerPass.resize(numPasses, NULL);
		defShaderPerPass.resize(numPasses, NULL);
		entitiesPerPass.resize(numPasses);
	}
	entitiesPerPass[pass].push_back(toAdd);
}

void GraphicsContext::RegisterCamera(Camera* toAdd, int pass){
	if(pass >= numPasses){
		numPasses = pass +1;
		cameraPerPass.resize(numPasses, NULL);
		defShaderPerPass.resize(numPasses, NULL);
		entitiesPerPass.resize(numPasses);
	}
	cameraPerPass[pass] = toAdd;
}

void GraphicsContext::RegisterShader(ShaderProgram* toAdd, int pass){
	if(pass >= numPasses){
		numPasses = pass +1;
		cameraPerPass.resize(numPasses, NULL);
		defShaderPerPass.resize(numPasses, NULL);
		entitiesPerPass.resize(numPasses);
	}
	defShaderPerPass[pass] = toAdd;
}
