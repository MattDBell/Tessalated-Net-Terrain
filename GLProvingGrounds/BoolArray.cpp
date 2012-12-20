#include "BoolArray.h"
#include <exception>

template<int numBools>
BoolArray<numBools>::BoolArray()
	: index(0)
{
	for(size_t i = 0; i < numBools/8 + 1; ++i)
		values[i] = 0;
}

template<int numBools>
BoolArray<numBools>& BoolArray<numBools>::operator=(bool rhs){
	int whichInt = index /8;
	int whichBit = index % 8;
	if(rhs){
		unsigned __int8 change = 1 << whichBit;
		values[whichInt] |= change;
		
	} else{
		unsigned __int8 change = (~1 << whichBit);
		values[whichInt] &= change;
	}
	return *this;
}

template <int numBools>
BoolArray<numBools>& BoolArray<numBools>::operator[](unsigned int index){
	this->index = index;
	return *this;
}

template <int numBools>
BoolArray<numBools>::operator bool(){
	int whichInt = index/8;
	int whichBit = index% 8;
	return ((values[whichInt] >> whichBit) & 1 )== 1;
}
template <int numBools>
void BoolArray<numBools>::SetAllTo(bool to){
	for(size_t i = 0; i < numBools/8 +1; ++i)
		values[i] = (to) ? 0xFF: 0;
}