#ifndef BOOLARRAY_H
#define BOOLARRAY_H


template<int numBools>
class BoolArray{
	unsigned int index;
	unsigned __int8 values[numBools/8 + 1];
	BoolArray(const BoolArray& rhs);
public:
	BoolArray();
	BoolArray& operator=(bool rhs);
	BoolArray& operator[](unsigned int index);
	void SetAllTo(bool to);
	operator bool();

};
#include "BoolArray.cpp"
#endif