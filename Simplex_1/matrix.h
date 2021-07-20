#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include "rational.h"

class matrix
{

	friend class matrix_vector;

protected:

	std::vector<std::vector<rational>> m = { {rational(0)} };
	size_t rows_in = 1;
	size_t cols_in = 1;

public:

	class matrix_vector;

	//////////////////////////////////////ACCESS MATRIX ELEMENTS, RWOS AND COLUMNS

	rational& at(size_t r, size_t c);
	const rational& at(size_t r, size_t c) const;

	/////////////////////////////////////PROPERTIES OF matrix

	bool empty() const;

	//Number of rows of matrix
	size_t rows() const;

	//Number of columns of matrix
	size_t columns() const;

	//Print matrix
	void print();

	/////////////////////////////////////////////////////////
	//MATRIX VECTOR
	class matrix_vector
	{

	private:

		matrix& m;
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

	std::vector<matrix_vector> matrix_vector_rows;
	std::vector<matrix_vector> matrix_vector_columns;

	std::vector<matrix_vector>& indicator_set_size(std::vector<matrix_vector>& v, int n);
	std::vector<matrix_vector>& indicator_increment(std::vector<matrix_vector>& v);
	std::vector<matrix_vector>& indicator_decrement(std::vector<matrix_vector>& v);

	/////////////////////////////////////////////////////////////////////////

	matrix(void);
	matrix(const matrix&);
	matrix(const std::vector<std::vector<rational>>&);

	matrix& operator =(const matrix& m_out);
	matrix& operator =(const std::vector<std::vector<rational>>&);

	matrix_vector& row(int i);
	const matrix_vector& row(int i) const { return matrix_vector_rows.at(i); }
	matrix_vector& col(int j);
	const matrix_vector& col(int j) const { return matrix_vector_columns.at(j); }
	matrix_vector& last_r();
	const matrix_vector& last_r() const { return matrix_vector_rows.back(); }
	matrix_vector& last_c();
	const matrix_vector& last_c() const { return matrix_vector_columns.back(); }

	/////////////////////////////////////METHODS THAT AFFECT THE matrix

//Sum a row with another row vector
	matrix& sum_r(size_t r, const std::vector<rational>&);

	//Sum a column with a vector
	matrix& sum_c(size_t c, const std::vector<rational>&);

	//Mult a row with a ratinoal
	matrix& mult_r(size_t r, const rational&);

	//Mult a column with a rational
	matrix& mult_c(size_t c, const rational&);

	//Pivot around a point
	matrix& piv(size_t r, size_t c);

	//Push a vector at the bottom
	matrix& push_r(const std::vector<rational>&);

	//Add column to the right
	matrix& push_c(const std::vector<rational>&);

	//Remove last row
	matrix& pop_r();

	//Remove last column
	matrix& pop_c();

	//matrix gauss();

	///////////////////////////////////THIS METHOD DOES NOT CHANGE THE matrix

	//Return transposed matrix
	matrix transpose();

	//ADDITIONAL FUNCIONS

};

//Checki f given vector is a canonical vector
bool is_canonical(const std::vector<rational>&);