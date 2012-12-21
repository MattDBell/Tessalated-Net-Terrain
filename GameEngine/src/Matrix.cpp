#ifndef MATRIX_CPP
#define MATRIX_CPP

#include "Matrix.h"
#include <math.h>
//arr[] ={	0, 0, 0, 0,
//			0, 0, 0, 0,
//			0, 0, 0, 0,
//			0, 0, 0, 0}T

template<int ROWS, int COLUMNS>
void	Matrix<ROWS, COLUMNS>::SetValue(int Row, int Column, int value){
	if(Row >= ROWS || Row < 0 || Column >= COLUMNS || Column < 0)
		return;
	value[Column*4 + 1] = value;
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS> Matrix<ROWS, COLUMNS>::Identity(){
	return MakeIdentity();
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS> Matrix<ROWS, COLUMNS>::MakeIdentity(){
	float newValues[ROWS * COLUMNS] = {0.0f};
	newValues[(COLUMNS-1) * ROWS + ROWS-1] = 1.0f;
	Matrix<ROWS-1, COLUMNS-1>::MakeIdentity<ROWS>(newValues);
	Matrix ret;
	ret.SetValues(newValues);
	return ret;

}



template<int ROWS, int COLUMNS>
template<int FULLROWS>
inline void Matrix<ROWS, COLUMNS>::MakeIdentity(float arr[]){
	arr[(COLUMNS-1)*(FULLROWS) + ROWS-1] = 1.0f;
	Matrix<ROWS-1, COLUMNS-1>::MakeIdentity<FULLROWS>(arr);
}

template<>
template<int FULLROWS>
void Matrix<1, 1>::MakeIdentity(float arr[]){
	arr[0] = 1.0f;
}





template<int ROWS, int COLUMNS>
MVector<ROWS> Matrix<ROWS, COLUMNS>::GetColumn (int column) const{
	MVector<ROWS> ret;
	float newValues[ROWS];
	for(int i = 0; i < ROWS; ++i)
		newValues[i] = values[column * ROWS +i];
	ret.SetValues(newValues);
	return ret;

}

template<int ROWS, int COLUMNS>
MVector<COLUMNS> Matrix<ROWS, COLUMNS>::GetRow (int row) const{
	MVector<COLUMNS> ret;
	float newValues[COLUMNS];
	for(int i = 0; i < COLUMNS; ++i)
		newValues[i] = values[i * COLUMNS + row];
	ret.SetValues(newValues);
	return ret;
}
template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS> Matrix<ROWS, COLUMNS>::Invalid(){
	Matrix<ROWS, COLUMNS> ret;
	float values[ROWS * COLUMNS] = {std::numeric_limits<float>::quiet_NaN()};
	ret.SetValues(values);
	return ret;

}
template<int ROWS, int COLUMNS>
void Matrix<ROWS, COLUMNS>::SetRow(int row, MVector<COLUMNS> values){
	if(row >= ROWS || row < 0)
		*this = Matrix<ROWS, COLUMNS>::Invalid();
	for(int i = 0; i < COLUMNS; ++i){
		this->values[i * ROWS +row] = values.GetValue(i);
	}
}
template<int ROWS, int COLUMNS>
void Matrix<ROWS, COLUMNS>::SetColumn(int column, MVector<ROWS> values){
	if(column >= COLUMNS || column < 0)
		*this = Invalid();
	for(int i = 0; i < ROWS; ++i){
		this->values[ROWS * column + i] = values.GetValue(i);
	}
}

template<int ROWS, int COLUMNS>
float Matrix<ROWS, COLUMNS>::GetValue (int Row, int Column) const{
	if(Row >= ROWS || Row < 0 || Column >= COLUMNS || Column < 0)
	return values[Column * ROWS + Row];
}

template<int ROWS, int COLUMNS>
void Matrix<ROWS, COLUMNS>::SetValues(const float* arr){
	for(int i = 0; i < ROWS* COLUMNS; ++i)
		values[i] = arr[i];
}

template<int ROWS, int COLUMNS>
void Matrix<ROWS, COLUMNS>::SetValues(float arr[]){
	for(int i = 0; i < ROWS* COLUMNS; ++i)
		values[i] = arr[i];
}

template<int ROWS, int COLUMNS>
template<int OTHERCOLUMNS>
Matrix<ROWS, OTHERCOLUMNS>	Matrix<ROWS, COLUMNS>::multiply(const Matrix<COLUMNS, OTHERCOLUMNS>& rhs)  const{
	Matrix<ROWS, OTHERCOLUMNS> ret;
	float newValues[ROWS * OTHERCOLUMNS] = {0};
	for(int i = 0; i < ROWS* OTHERCOLUMNS; ++i){
		newValues[i] = 0.0f;
		int column = i / ROWS;
		int row = i % ROWS;
		for(int j = 0; j < COLUMNS; ++j)
		{
			float valueThis = values[ j* ROWS + row];
			float valueThat = rhs.values[j + column* OTHERCOLUMNS];
			newValues[i] += valueThis * valueThat;
		}

	}
	ret.SetValues(newValues);
	return ret;
}


template<int ROWS, int COLUMNS>
MVector<ROWS>				Matrix<ROWS, COLUMNS>::multiply(const MVector<ROWS> rhs) const{
	Matrix<ROWS, 1> matrix;
	matrix.SetColumn(0, rhs);
	matrix = this->multiply(matrix);
	return matrix.GetColumn(0);
}


template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>		Matrix<ROWS, COLUMNS>::multiply(float scalar) const{
	float newValues[ROWS*COLUMNS];
	for(int i = 0; i < ROWS*COLUMNS; ++i)
		newValues[i] = values[i] * scalar;
	Matrix<ROWS, COLUMNS> ret;
	ret.SetValues(newValues);
	return ret;
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>		Matrix<ROWS, COLUMNS>::divide(float scalar) const{
	return this->multiply(1/scalar);
}

template<int ROWS, int COLUMNS>
Matrix<COLUMNS, ROWS>		Matrix<ROWS, COLUMNS>::Transposed() const{
	Matrix<COLUMNS, ROWS> ret = Matrix<COLUMNS, ROWS>();
	for (int i = 0; i < COLUMNS; ++i){
		ret.SetRow(i, this->GetColumn(i));
	}
	return ret;
}

// Not Implemented yet (Shocker, I know);
template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>		Matrix<ROWS, COLUMNS>::Inversed() const{//HOLD OFF FO' NOW
	if(ROWS != COLUMNS)
		return Matrix<ROWS, COLUMNS>::Invalid();

	Matrix ret= Matrix<COLUMNS, ROWS>::Identity();
	//do stuff
	return ret;
}
template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>		Matrix<ROWS, COLUMNS>::operator+( const Matrix<ROWS, COLUMNS> &rhs) const{
	Matrix<ROWS, COLUMNS> ret= Matrix<COLUMNS, ROWS>();
	float[ROWS* COLUMNS] newValues;
	for(int i = 0; i < ROWS* COLUMNS; ++i)
		newValues[i] = values[i] + rhs.values[i];
	ret.SetValues(newValues);
	return ret;
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>		Matrix<ROWS, COLUMNS>::operator-( const Matrix<ROWS, COLUMNS> &rhs) const{
	Matrix<ROWS, COLUMNS> ret= Matrix<COLUMNS, ROWS>();
	float[ROWS* COLUMNS] newValues;
	for(int i = 0; i < ROWS* COLUMNS; ++i)
		newValues[i] = values[i] - rhs.values[i];
	ret.SetValues(newValues);
	return ret;
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>&		Matrix<ROWS, COLUMNS>::operator+=( const Matrix<ROWS, COLUMNS> &rhs){
	*this = *this + rhs;
}

template<int ROWS, int COLUMNS>
Matrix<ROWS, COLUMNS>&		Matrix<ROWS, COLUMNS>::operator-=( const Matrix<ROWS, COLUMNS> &rhs){
	*this = *this + rhs;
}



template<int ROWS, int COLUMNS>
void						Matrix<ROWS, COLUMNS>::Invert(){
	*this = this->Inversed();
}

template<int ROWS, int COLUMNS>
void						Matrix<ROWS, COLUMNS>::Transpose(){
	*this = this->Transposed();
}


template<int ROWS, int COLUMNS>
bool						Matrix<ROWS, COLUMNS>::is(const Matrix<ROWS, COLUMNS> &rhs) const{
	return this == &rhs;
}

template<int ROWS, int COLUMNS>
bool						Matrix<ROWS, COLUMNS>::operator==(const Matrix<ROWS, COLUMNS> &rhs) const{
	if(this->is(rhs))
		return true;
	for(int i = 0; i < ROWS * COLUMNS; ++i)
		if(values[i] != rhs.values[i])
			return false;
	return true;
}

template<int ROWS, int COLUMNS>
bool						Matrix<ROWS, COLUMNS>::operator!=(const Matrix<ROWS, COLUMNS> &rhs) const{
	return !(*this == rhs);
}


template<int ROWS, int COLUMNS>
template<int byRows, int byColumns> Matrix<ROWS + byRows, COLUMNS + byColumns> Matrix<ROWS, COLUMNS>::IncreasedDimension(float fillValue) const{
	Matrix<ROWS + byRows, COLUMNS + byColumns> ret;
	for(int i = 0; i < ROWS; ++i){
		ret.SetRow(i, GetRow(i).IncreasedDimension<byColumns>(fillValue));
	}
	for(int i = ROWS; i < ROWS + byROWS; ++i){
		ret.SetRow(MVector<COLUMNS+byColumns>(fillValue));
	}
	return ret;
}




#endif