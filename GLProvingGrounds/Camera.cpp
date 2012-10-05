#include "Camera.h"
#include <cmath>


Camera::Camera(){
	matrices.view = Matrix<4, 4>::Identity();
	matrices.proj = Matrix<4, 4>::Identity();

}

const Matrix<4, 4>& Camera::GetView(){
	return matrices.view;
}

const Matrix<4, 4>& Camera::GetProj(){
	return matrices.proj;
}
void Camera::LookAt(MVector<3> position, MVector<3> target, MVector<3> Up){
	this->position = position;
	
	
	//Used to ease inverting
	Matrix<4, 4> orientation;
	Matrix<4, 4> translation;

	//Get forward direction
	forward = position - target;
	forward.Normalize(); // In place normalization

	//Normalize up
	up = Up.Normalized(); // returns a normalized copy
	
	//Gram-Schmidt
	MVector<3> projection = forward * (up.Dot(forward));
	up -= projection;
	up.Normalize();
	//Gram-Schmidt, considered doing it again to ensure precision
	//proj = forward * (up.Dot(forward));
	//up -= proj;

	//Get right vector
	right = up.Cross(forward * -1);
	
	//Set the orientation vector
	orientation.SetColumn(0, right.IncreasedDimensions<1>(0));	//IncreasedDimensions returns an MVector with a templated
																//Increase in dimensions and fills the new slots with the
																//Argument provided

																//SetColumn sets the values of the column to those of the MVector
	orientation.SetColumn(1, up.IncreasedDimensions<1>(0));
	orientation.SetColumn(2, (forward*-1).IncreasedDimensions<1>(0));
	orientation.SetColumn(3, MVector<4>(0)(0)(0)(1));	//This is a very cheat way to have a variadic constructor
														//The MVector is first created with all the values set to
														//the first argument (0).  The operator() keeps track of
														//where it should place the next value and then places it
														//there.  Resulting (in this case) in (0, 0, 0, 1)
	
	translation = Matrix<4, 4>::Identity();
	translation.SetColumn(3, position.IncreasedDimensions<1>(1)); // translation Matrix, simple enough
	MVector<4> thereAndBack = MVector<4>(1)(1)(1)(1);
	world = translation.multiply(orientation); // This is the cameras object - world matrix
	thereAndBack = world.multiply(thereAndBack);

	translation.SetColumn(3, (position * -1).IncreasedDimensions<1>(1));  // Invert the translation matrix

	matrices.view = orientation.Transposed().multiply(translation); // Orientation matrix inverse == it's transpose.  Then reverse order
	thereAndBack = view.multiply(thereAndBack);
	thereAndBack = world.multiply(thereAndBack);
	thereAndBack = view.multiply(thereAndBack);
	Matrix<4, 4> id = world.multiply(view);
}
void Camera::SetProj(float nearClip, float farClip, float fieldOfViewY, MVector<2> aspectRatio){
	zoomY = 1/tan(fieldOfViewY/2);
	float zoomX = zoomY * aspectRatio[1] /aspectRatio[0];
	
	
	float projValues[16] = {	zoomX,	0,		0,		0,   
								0,		zoomY,	0,		0,	
								0,		0,		(farClip + nearClip)/(farClip-nearClip), 1,
								0,		0,		(2 * nearClip * farClip) /( nearClip - farClip),	0}; 
	matrices.proj.SetValues(projValues);
	//proj.Transpose();

}
void Camera::SetCurrent(){
	Camera::cameraUBO.Update(&matrices);
}