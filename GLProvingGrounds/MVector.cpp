#ifndef VECTOR_CPP
#define VECTOR_CPP

#include "MVector.h"
#include <math.h>
#include <limits>

template<int ROWS>
float * MVector<ROWS>::GetValues(){
	return &values[0];
}
//template<int ROWS>
//template<int next, int value, int... nextvalues>
//MVector<ROWS>::MVector(){
//	values[i] = value;
//	MVector<ROWS>::MVector<next, nextvalues>();
//}



template <int ROWS>
MVector<ROWS>& MVector<ROWS>::operator()(const float inBrackets){
	if(index >= ROWS)
		return *this = MVector<ROWS>::Invalid();
	values[index++] = inBrackets;

	return *this;
}

template <int ROWS>
float			MVector<ROWS>::GetValue(int i) const {
	if(i >= ROWS || i < 0)
		return std::numeric_limits<float>::quiet_NaN();
	return values[i];
}

template <int ROWS>
void			MVector<ROWS>::SetValue(int i, float to){
	if(i >= ROWS || i < 0)
		return;
	values[i] = to;
}

template <int ROWS>
void			MVector<ROWS>::SetValues(float arr[]){
	for(int i = 0; i < ROWS; ++i)
		values[i] = arr[i];
}
//template <int ROWS>
//void			MVector<ROWS>::SetValues(float  * const arr ){
//	for(int i = 0; i < ROWS; ++i)
//		values[i] = arr[i];
//}

template <int ROWS>
float			MVector<ROWS>::Dot(const MVector<ROWS> &other){
	float ret = 0.0f;
	for(int i = 0; i < ROWS; ++i)
	{
		ret += values[i] * other.values[i];
	}
	return ret;
}

template <int ROWS>
MVector<ROWS>	MVector<ROWS>::Normalized(){
	return *this/this->Length();
}

template <int ROWS>
void			MVector<ROWS>::Normalize(){
	*this = this->Normalized();
}

template <int ROWS>
float			MVector<ROWS>::LengthSquared(){
	
	return this->Dot(*this);
}

template <int ROWS>
float			MVector<ROWS>::Length(){
	return sqrt(this->Dot(*this));
}
	

template <int ROWS>
MVector<ROWS>	MVector<ROWS>::operator+(const MVector<ROWS> &rhs){
	MVector ret = MVector();
	for(int i = 0; i < ROWS; ++i)
		ret.values[i] = this->values[i] + rhs.values[i];
	return ret;
}
template <int ROWS>
MVector<ROWS>	MVector<ROWS>::operator-(const MVector<ROWS> &rhs){
	MVector ret  = MVector();
	for(int i = 0; i < ROWS; ++i)
		ret.values[i] = this->values[i] - rhs.values[i];
	return ret;
}
template <int ROWS>
MVector<ROWS>	MVector<ROWS>::operator*(const float rhs){
	MVector ret = MVector();
	for(int i = 0; i < ROWS; ++i)
		ret.values[i] = this->values[i] *rhs;
	return ret;
}
template <int ROWS>
MVector<ROWS>	MVector<ROWS>::operator/(const float rhs){
	return *this * (1/rhs);
}

template <int ROWS>
bool			MVector<ROWS>::operator==(const MVector<ROWS> &rhs){
	if (this->is(rhs))
		return true;
	for(int i = 0; i < ROWS; ++i)
		if(values[i] != rhs.values[i])
			return false;
	return true;
}

template <int ROWS>
bool			MVector<ROWS>::operator!=(const MVector<ROWS> &rhs){
	return !(*this == rhs);
}

template <int ROWS>
bool			MVector<ROWS>::is(const MVector<ROWS> &rhs){
	return this == &rhs;
}
template<int ROWS>
MVector<ROWS> MVector<ROWS>::Cross(const MVector<ROWS> &rhs){
	return MVector<ROWS>::Invalid();
}
template<>
MVector<3> MVector<3>::Cross(const MVector<3> & rhs);


template<int ROWS>
MVector<ROWS> MVector<ROWS>::Invalid()
{
	MVector<ROWS> ret;
	float values[ROWS] = {std::numeric_limits<float>::signaling_NaN()};
	ret.SetValues(values);
	return ret;
}

template<int ROWS>
bool MVector<ROWS>::isInvalid(){
	if(ROWS < 1)
		return true;
	return values[0].isNan();
}

template<int ROWS>
template<int ByRows>
MVector<ROWS + ByRows>  MVector<ROWS>::IncreasedDimensions(float fill){
	MVector<ROWS+ByRows> ret;
	float newValues[ROWS+ByRows] = {0};
	for(size_t i = 0; i < ROWS; ++i)
		newValues[i] = values[i];
	for(size_t i = ROWS; i < ROWS + ByRows; ++i)
		newValues[i] = fill;
	ret.SetValues(newValues);
	return ret;
}

#endif