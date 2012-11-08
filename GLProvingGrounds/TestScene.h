#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Camera.h"
#include "TestCube.h"
#include "Input.h"
#include "Light.h"
#include "MarchingCubeAsteroid.h"
const float PI = 3.14159265f;

struct TestScene{
	Light* light;
	Light* light2;
	TestCube * cube;
	MarchingCubeAsteroid* asteroid;

	Camera * cam;
	MVector<2>	AspectRatio;
	MVector<3>  lookAt;
	float elevation, azimuth, distance;
	TestScene(GraphicsContext * context)
	{
		MVector<4> lightPos = { 10, 10, 10, 1 };
		MVector<4> lightColor = {1.0f, 1.0f, 1.0f, 0.0f};
		
		light = new Light(lightPos, lightColor, 0x3ff00000, 0);
		light2 = new Light(lightPos * -1, lightColor, 0x3ff00000, 1);
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

		asteroid = MarchingCubeAsteroid::Create();
		asteroid->Initialize();
		context->RegisterGraphComp(asteroid, 0);
		context->RegisterGraphComp(cube, 0);
	}
	~TestScene(){
		delete cam;
		delete cube;
		delete light;
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
		elevation = elevation > 0.9f ? 0.9f : elevation < -0.9f ? -0.9f : elevation;
		MVector<3> camPos= { cos(elevation) * cos(azimuth) * distance, sin(elevation)* distance, cos(elevation) * sin(azimuth)* distance};
		MVector<3> upVec = {0, 1, 0};
		cam->LookAt(camPos, lookAt, upVec);
		cam->SetCurrent();
	}
};
#endif