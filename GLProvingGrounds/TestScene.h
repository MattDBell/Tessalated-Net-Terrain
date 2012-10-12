#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Camera.h"
#include "TestCube.h"
#include "Input.h"


struct TestScene{
	
	Camera * cam;
	TestCube * cube;
	MVector<2>	AspectRatio;
	MVector<3>  lookAt;
	
	TestScene(GraphicsContext * context){
		cam = new Camera();
		MVector<3> camStart;
		MVector<3> upVec;
		
		float cStart[] = {0, 2, -10 };
		float lookAtArr[] = {0, 0, 0};
		float upArr[] = {0, 1, 0};
		camStart.SetValues(cStart);

		lookAt.SetValues( lookAtArr) ;
		upVec.SetValues( upArr);

		AspectRatio.SetValue(0, 1);
		AspectRatio.SetValue(1, 1);
		cam->SetProj(0.01f, 1000.0f, 45, AspectRatio );
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