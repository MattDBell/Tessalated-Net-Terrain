#ifndef MATRIX_H
#define MATRIX_H

#include "MVector.h"



template <int ROWS, int COLUMNS>
struct Matrix{
	//static_assert(ROWS > 0 && COLUMNS >0, "ROWS and COLUMNS must be positive non-zero integers");
	float values[ROWS *COLUMNS];
	static Matrix<ROWS, COLUMNS> MakeIdentity();

	template<int FULLROWS>
	static void MakeIdentity(float arr[]);
	static Matrix<ROWS, COLUMNS> Identity();
	static Matrix<ROWS, COLUMNS> Invalid();

	int GetNumRows() { return ROWS;}
	int GetNumColumns() { return COLUMNS;}

	MVector<ROWS> GetColumn (int column) const;
	MVector<COLUMNS> GetRow (int row) const;

	void SetRow(int row, MVector<COLUMNS> values);
	void SetColumn(int row, MVector<ROWS> values);

	float GetValue (int Row, int Column) const;
	void  SetValue (int Row, int Column, int value);

	void SetValues(float arr[]);
	void SetValues(const float* arr);
	
	const float* MatrixPointer() const {
		return &values[0];
	}

	template<int OTHERCOLUMNS>
	Matrix<ROWS, OTHERCOLUMNS>	multiply (const Matrix<COLUMNS, OTHERCOLUMNS>& rhs) const;

	MVector<ROWS>				multiply (const MVector<ROWS> rhs) const;

	Matrix<ROWS, COLUMNS>		multiply (float scalar) const;
	Matrix<ROWS, COLUMNS>		divide (float scalar) const;
	Matrix<COLUMNS, ROWS>		Transposed () const;
	Matrix<ROWS, COLUMNS>		Inversed () const;

	Matrix<ROWS, COLUMNS>		operator+( const Matrix<ROWS, COLUMNS> &rhs) const;
	Matrix<ROWS, COLUMNS>		operator-( const Matrix<ROWS, COLUMNS> &rhs) const;
	/*Matrix<ROWS, COLUMNS>		operator*( const Matrix<ROWS, COLUMNS> &rhs) const;
	Matrix<ROWS, COLUMNS>&		operator*=(const Matrix<ROWS, COLUMNS> &rhs);*/
	Matrix<ROWS, COLUMNS>&		operator+=( const Matrix<ROWS, COLUMNS> &rhs);
	Matrix<ROWS, COLUMNS>&		operator-=( const Matrix<ROWS, COLUMNS> &rhs);

	void						Invert();
	void						Transpose();

	bool						is(const Matrix<ROWS, COLUMNS> &rhs) const;
	bool						operator==(const Matrix<ROWS, COLUMNS> &rhs) const;
	bool						operator!=(const Matrix<ROWS, COLUMNS> &rhs) const;

	template<int byRows, int byColumns>
	Matrix<ROWS+byRows, COLUMNS+byColumns>	IncreasedDimension(float fillValue) const;
};



Matrix<4, 4>	Rotate(MVector<3> axis, float angle);
Matrix<4, 4>	Rotate(MVector<4> axis, float angle);
Matrix<4, 4>	Scale(float scale);
Matrix<4, 4>	Translate(MVector<3> translation);
Matrix<4, 4>	Translate(MVector<4> translation);

#include "Matrix.cpp"
#endif