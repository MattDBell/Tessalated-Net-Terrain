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
	//Light* light2;
	TestCube * cube;
	MarchingCubeAsteroid* asteroid;

	Camera * cam;
	MVector<2>	AspectRatio;
	MVector<3>  lookAt;
	float elevation, azimuth, distance;
	float lightangle;
	TestScene(GraphicsContext * context)
	{
		lightangle = 0.0f;

		MVector<4> lightPos = { 10 * cos(lightangle), 10, 10 * sin(lightangle), 1 };
		MVector<4> lightColor = {1.0f, 1.0f, 1.0f, 0.0f};
		
		light = new Light(lightPos, lightColor, 0x3ff00000, 0);
		//light2 = new Light(lightPos * -1, lightColor, 0x3ff00000, 1);
		cam = new Camera();
		elevation = PI * 0.25f;
		azimuth = 0.0f;
		distance = 30.0f;
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

		asteroid = MarchingCubeAsteroid::Create("Textures\\", "asteroid");
		asteroid->Initialize();
		
		context->RegisterGraphComp(cube, 0);
		context->RegisterGraphComp(asteroid, 0);
	}
	~TestScene(){
		delete cam;
		delete cube;
		delete light;
		//delete light2;
		delete asteroid;
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
			distance += dT * 10.0f;
		}
		if(Input::Get()->GetKey('E')){
			distance-= dT* 10.0f;
		} 
		if(Input::Get()->GetKey('1')){
			asteroid->SetState(MESS);
		}
		if(Input::Get()->GetKey('2')){
			asteroid->SetState(FACETEDSPHERE);
		}
		if(Input::Get()->GetKey('3')){
			asteroid->SetState(SPHERE);
		}
		if(Input::Get()->GetKey('4')){ //Because the keyboard has 0 at the wrong place
			asteroid->SetState(SINE_WAVE_THING);
		}
		if(Input::Get()->GetKey('5')){
			asteroid->SetState(TERRAIN);
		}
		
		//If user clicks
		//Cast a ray find point of contact with MC
		//Do a sphere cast around that point of brushSize
		//lower density depending on collision

		elevation = elevation > 0.9f ? 0.9f : elevation < -0.9f ? -0.9f : elevation;
		if(azimuth > 2 * PI) azimuth -= 2 * PI;
		MVector<3> camPos= { cos(elevation) * cos(azimuth) * distance, sin(elevation)* distance, cos(elevation) * sin(azimuth)* distance};
		MVector<3> upVec = {0, 1, 0};
		cam->LookAt(camPos, lookAt, upVec);
		cam->SetCurrent();

		lightangle += dT;
		if(lightangle > 2 * PI) lightangle -= 2 * PI;
		MVector<4> lightPos = { 10 * cos(lightangle), 10, 10 * sin(lightangle), 1 };
		light->MoveTo(lightPos);

		asteroid->Update(dT);
	}
};
#endif