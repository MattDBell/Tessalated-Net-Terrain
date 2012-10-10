#ifndef TESTSCENE_H
#define TESTSCENE_H
#include "Camera.h"
#include "Input.h"


struct TestScene{
	
	Camera * cam;
	~TestScene(){
		cam = new Camera();
		cam->SetProj(0.01f, 1000f, 45, MVector<2>(1)(1));
		cam->LookAt(MVector<3>(0)(2)(-10), MVector<3>(0), MVector<3>(0)(1)(0)); 
	}
	void Update(float dT){
		//if(Input::Get()->GetKey('A')){
		//	//MoveCamera
		//}
	}
};
#endif