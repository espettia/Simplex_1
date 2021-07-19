#pragma once
#include <iostream>
#include <vector>
#include <cassert>
#include "rational.h"

class matrix
{

	friend class matrix_vector;

protected:

	std::vector<std::vector<rational>> m = { {rational(0)} };
	size_t rows_in;
	size_t cols_in;

public:

	class matrix_vector;

	//////////////////////////////////////ACCESS MATRIX ELEMENTS, RWOS AND COLUMNS

	rational& at(size_t r, size_t c);

	/////////////////////////////////////PROPERTIES OF matrix

	bool empty();

	//Number of rows of matrix
	size_t rows();

	//Number of columns of matrix
	size_t columns();

	//Print matrix
	void print();

	/////////////////////////////////////////////////////////
	//MATRIX VECTOR
	class matrix_vector
	{

	private:

		std::vector<std::vector<rational>>& m;
		bool r_c;
		size_t pos;

	public:

		matrix_vector(matrix& m_out, bool r_c_out, size_t i);

		rational& operator [](int i);
		size_t size() const;
		matrix_vector& operator = (const matrix_vector&);
		matrix_vector& operator = (const std::vector<rational>&);
		const rational& operator [](int i) const;
		rational& front();
		rational& back();
		std::vector<rational> vector() const;
		void print();
	};

	std::vector<matrix_vector> matrix_vector_rows = { matrix_vector(*this, 0, 0) };
	std::vector<matrix_vector> matrix_vector_columns = { matrix_vector(*this, 1, 0) };

	/////////////////////////////////////////////////////////////////////////

	matrix(void);
	matrix(std::vector<std::vector<rational>>);
	matrix operator =(matrix m_out);
	matrix operator =(std::vector<std::vector<rational>>);

	matrix_vector row(int i);

	matrix_vector col(int j);
	matrix_vector last_r();

	matrix_vector last_c();

	/////////////////////////////////////METHODS THAT AFFECT THE matrix

//Sum a row with another row vector
	matrix sum_r(size_t r, std::vector<rational>);

	//Sum a column with a vector
	matrix sum_c(size_t c, std::vector<rational>);

	//Mult a row with a ratinoal
	matrix mult_r(size_t r, rational);

	//Mult a column with a rational
	matrix mult_c(size_t c, rational);

	//Pivot around a point
	matrix piv(size_t r, size_t c);

	//Push a vector at the bottom
	matrix push_r(std::vector<rational>);

	//Add column to the right
	matrix push_c(std::vector<rational>);

	//Remove last row
	matrix pop_r();

	//Remove last column
	matrix pop_c();

	//matrix gauss();

	///////////////////////////////////THIS METHOD DOES NOT CHANGE THE matrix

	//Return transposed matrix
	matrix transpose();

	//ADDITIONAL FUNCIONS

};

//Checki f given vector is a canonical vector
bool is_canonical(std::vector<rational> v);