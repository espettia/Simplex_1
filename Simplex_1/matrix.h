#pragma once
#include <iostream>
#include <vector>
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

	std::vector<rational> mult(rational, std::vector<rational>);

	rational& at(size_t r, size_t c);

	std::vector<rational>& row(size_t r);
	std::vector<rational>& col(size_t c);

	bool empty();

	size_t rows();

	size_t columns();

	matrix sum_r(size_t r, std::vector<rational>);

	matrix sum_c(size_t c, std::vector<rational>);

	matrix piv(size_t r, size_t c);

	void print();

	matrix push_r(std::vector<rational>);

	matrix push_c(std::vector<rational>);

	matrix pop_r();

	matrix pop_c();
	
	//matrix gauss();

	matrix transpose();

};

