#ifndef CAMERA_H
#define CAMERA_H
#include "UniformBufferObject.h"
#include "Matrix.h"
class Camera{
	struct cameraMatrices{
		Matrix<4, 4> view;
		Matrix<4, 4> proj;
	};
	static UniformBufferObject<cameraMatrices>* cameraUBO;
	cameraMatrices matrices;
	Matrix<4, 4> world;

	Matrix<4, 4> view;
	Matrix<4, 4> proj;
	
	MVector<3> position;
	MVector<3> forward;
	MVector<3> right;
	MVector<3> up;
	MVector<2> aspect;
	float zoomY, nearC, farC;
public:
	Camera();
	void LookAt(MVector<3> &position, MVector<3> &target, MVector<3> &Up);
	void SetProj(float nearClip, float farClip, float fieldOfView, MVector<2> &aspectRatio);
	const Matrix<4, 4>& GetView();
	const Matrix<4, 4>& GetProj();
	void SetCurrent();
	MVector<3>& GetPosition(){return position;}
};

#endif