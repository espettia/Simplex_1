#pragma once
#include <iostream>
#include <vector>
#include <functional>
#include "rational.h"

class matrix
{
private:

	std::vector<std::vector<rational>> m = { {rational(0)} };

public:

	matrix(void);
	matrix(std::vector<std::vector<rational>>);
	matrix operator =(matrix m_out);
	matrix operator =(std::vector<std::vector<rational>>);

	//////////////////////////////////////ACCESS MATRIX ELEMENTS, RWOS AND COLUMNS

	rational& at(size_t r, size_t c);

	std::vector<rational>& row(size_t r);

	std::vector<rational> col(size_t c);


	/////////////////////////////////////PROPERTIES OF MATRIX

	bool empty();

	//Number of rows of matrix
	size_t rows();

	//Number of columns of matrix
	size_t columns();

	//To access last row
	std::vector<rational>& last_r();

	//To access last column;
	std::vector<rational> last_c();

	/////////////////////////////////////METHODS THAT AFFECT THE MATRIX

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

	//Print matrix
	void print();

	//Push a vector at the bottom
	matrix push_r(std::vector<rational>);

	//Add column to the right
	matrix push_c(std::vector<rational>);

	//Remove last row
	matrix pop_r();

	//Remove last column
	matrix pop_c();

	//matrix gauss();

	///////////////////////////////////THIS METHOD DOES NOT CHANGE THE MATRIX

	//Return transposed matrix
	matrix transpose();

};

//Checki f given vector is a canonical vector
bool is_canonical(std::vector<rational> v);

