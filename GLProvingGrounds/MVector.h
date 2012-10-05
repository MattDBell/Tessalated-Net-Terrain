#ifndef VECTOR_H
#define VECTOR_H


template <int ROWS>
struct MVector{
	//static_assert(ROWS>0, "ROWS Must be a positive non-zero");
private:
	
	float values[ROWS];
	int index;
public:
	MVector();
	MVector(float fillOrFirst);
	~MVector();

	static MVector<ROWS> Invalid();
	bool isInvalid();

	float GetValue(int index) const ;
	float * GetValues();
	void SetValue(int index, float to);
//	void SetValues(float  arr[]);
	void SetValues(float  * const arr );
	float Dot(MVector<ROWS> &other);
	MVector<ROWS> Cross(MVector<ROWS> &other);
	
	MVector<ROWS> Normalized();
	void Normalize();
	
	float LengthSquared();
	float Length();
	
	MVector<ROWS>	operator+(const MVector<ROWS> &rhs);
	MVector<ROWS>	operator-(const MVector<ROWS> &rhs);
	MVector<ROWS>	operator*(float rhs);
	MVector<ROWS>	operator/(float rhs);
	MVector<ROWS>&	operator=(const MVector<ROWS> &rhs);
	MVector<ROWS>&	operator+= (const MVector<ROWS> &rhs);
	MVector<ROWS>&	operator-= (const MVector<ROWS> &rhs);
	MVector<ROWS>&  operator()(float inBrackets);
	float			operator[](int index);
	
	
	bool			operator==(const MVector<ROWS> &rhs);
	bool			operator!=(const MVector<ROWS> &rhs);
	bool			is(const MVector<ROWS> &rhs);

	template<int ByRows>
	MVector<ROWS + ByRows>  IncreasedDimensions(float fill);
};



#include "MVector.cpp"
#endif