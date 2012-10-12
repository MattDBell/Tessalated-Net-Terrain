#ifndef VECTOR_H
#define VECTOR_H

//Refactor to be POD with no index
template <int ROWS>
struct MVector{
	//static_assert(ROWS>0, "ROWS Must be a positive non-zero");
	float values[ROWS];
	static MVector<ROWS> Invalid();
	bool isInvalid();

	float GetValue(int index) const ;
	float * GetValues();
	void SetValue(int index, float to);
//	void SetValues(float  arr[]);
	void SetValues(float  * const arr );
	float Dot(const MVector<ROWS> &other);
	MVector<ROWS> Cross(const MVector<ROWS> &other);
	
	MVector<ROWS> Normalized();
	void Normalize();
	
	float LengthSquared();
	float Length();
	
	MVector<ROWS>	operator+(const MVector<ROWS> &rhs);
	MVector<ROWS>	operator-(const MVector<ROWS> &rhs);
	MVector<ROWS>	operator*(const float rhs);
	MVector<ROWS>	operator/(const float rhs);
	MVector<ROWS>&  operator()(const float inBrackets);
	
	
	bool			operator==(const MVector<ROWS> &rhs);
	bool			operator!=(const MVector<ROWS> &rhs);
	bool			is(const MVector<ROWS> &rhs);

	template<int ByRows>
	MVector<ROWS + ByRows>  IncreasedDimensions(float fill);
};



#include "MVector.cpp"
#endif