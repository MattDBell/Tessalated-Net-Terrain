#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Camera.h"
#include "TestCube.h"
#include "Input.h"

const float PI = 3.14159265f;

struct TestScene{
	
	Camera * cam;
	TestCube * cube;
	MVector<2>	AspectRatio;
	MVector<3>  lookAt;
	float elevation, azimuth, distance;
	TestScene(GraphicsContext * context){
		cam = new Camera();
		elevation = PI * 0.25f;
		azimuth = 0.0f;
		distance = 10.0f;
		MVector<3> camStart = { cos(elevation) * cos(azimuth) * distance, sin(elevation)* distance, cos(elevation) * sin(azimuth)* distance};
		MVector<3> upVec = {0, 1, 0};
				
		float lookAtArr[] = {0, 0, 0};
		lookAt.SetValues( lookAtArr) ;	

		AspectRatio.SetValue(0, 1);
		AspectRatio.SetValue(1, 1);
		cam->SetProj(0.01f, 1000.0f, (float)(30.0f/ 180 * PI) , AspectRatio );
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
	void Update(float dT){

		if(Input::Get()->GetKey('A')){
			azimuth += dT;
		}
		if(Input::Get()->GetKey('D')){
			azimuth -= dT;
		}
		if(Input::Get()->GetKey('W')){
			elevation += dT;
		}
		if(Input::Get()->GetKey('S')){
			elevation -= dT;
		}
		if(Input::Get()->GetKey('Q')){
			distance += dT;
		}
		if(Input::Get()->GetKey('E')){
			distance-= dT;
		}
		MVector<3> camPos= { cos(elevation) * cos(azimuth) * distance, sin(elevation)* distance, cos(elevation) * sin(azimuth)* distance};
		MVector<3> upVec = {0, 1, 0};
		cam->LookAt(camPos, lookAt, upVec);
		cam->SetCurrent();
	}
};
#endif