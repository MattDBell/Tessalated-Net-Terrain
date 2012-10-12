#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Camera.h"
#include "TestCube.h"
#include "Input.h"

const double PI = 3.14159265;

struct TestScene{
	
	Camera * cam;
	TestCube * cube;
	MVector<2>	AspectRatio;
	MVector<3>  lookAt;
	
	TestScene(GraphicsContext * context){
		cam = new Camera();
		MVector<3> camStart = { 0, 2, -10};
		MVector<3> upVec = {0, 1, 0};
				
		float lookAtArr[] = {0, 0, 0};
		lookAt.SetValues( lookAtArr) ;	

		AspectRatio.SetValue(0, 1);
		AspectRatio.SetValue(1, 1);
		cam->SetProj(0.01f, 1000.0f, (float)(45.0f/ 180 * PI) , AspectRatio );
		cam->LookAt(camStart, lookAt, upVec);

		context->RegisterCamera(cam, 0);

		cube = TestCube::CreateCube();
		cube->Initialize();
		context->RegisterGraphComp(cube, 0);
	}
	~TestScene(){
		delete cam;
		delete cube;
	}
	void Update(float){

		//if(Input::Get()->GetKey('A')){
		//	//MoveCamera
		//}
	}
};
#endif