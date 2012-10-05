#include "Matrix.h"


Matrix<4, 4>	Rotate(MVector<3> axis, float angle){
	float cA = cos(angle);
	float sA = sin(angle);
	float x = axis.GetValue(0);
	float y = axis.GetValue(1);
	float z = axis.GetValue(2);

	float newValues[] = {	cA + x*x*(1-cA),	x*y*(1-cA) - z*sA,		x*z*(1-cA) + y*sA,	0.0f,
							y*x*(1-cA) + z*sA,	cA + y*y*(1-cA),		y*z*(1-cA) - x*sA,	0.0f,
							z*x*(1-cA) - y*sA,	z*y*(1-cA) + x*sA,		cA + z*z*(1-cA),	0.0f,
							0.0f,				0.0f,					0.0f,				1.0f
	};
	Matrix<4, 4> ret;
	ret.SetValues(newValues);
	ret.Transpose();
	return ret;
}
Matrix<4, 4>	Rotate(MVector<4> axis, float angle){
	MVector<3> toPass;
	float components[] = {axis.GetValue(0), axis.GetValue(1), axis.GetValue(2)};
	toPass.SetValues(components);
	return Rotate(toPass, angle);
}
Matrix<4, 4>	Scale(float scale){
	float newValues[] = {	scale,	0.0f,	0.0f,	0.0f,
							0.0f,	scale,	0.0f,	0.0f,
							0.0f,	0.0f,	scale,	0.0f,
							0.0f,	0.0f,	0.0f,	1.0f
	};
	Matrix<4,4> ret;
	ret.SetValues(newValues);
	return ret;
}
Matrix<4, 4>	Translate(MVector<3> translation){
	float newValues[] = {	1.0f,	0.0f,	0.0f,	translation.GetValue(0),
							0.0f,	1.0f,	0.0f,	translation.GetValue(1),
							0.0f,	0.0f,	1.0f,	translation.GetValue(2),
							0.0f,	0.0f,	0.0f,	1.0f
	};
	Matrix<4,4> ret;
	ret.SetValues(newValues);
	ret.Transpose();
	return ret;
}
Matrix<4, 4>	Translate(MVector<4> translation){
	float components[] = {translation.GetValue(0),translation.GetValue(1),translation.GetValue(2)};
	MVector<3> toPass;
	toPass.SetValues(components);
	return Translate(toPass);
}

template<>
MVector<3> MVector<3>::Cross(MVector<3> &rhs){
	MVector<3> ret;
	for(int i = 0; i < 3; ++i){
		ret.SetValue(i,GetValue((i+1)%3) * rhs.GetValue((i+2)%3) - GetValue((i+2)%3) * rhs.GetValue((i+1)%3));
	}
	return ret;
}
template<>
Matrix<1, 1> Matrix<1, 1>::MakeIdentity(){
	float newValues[1] = {1.0f};
	Matrix ret;
	ret.SetValues(newValues);
	return ret;
}
