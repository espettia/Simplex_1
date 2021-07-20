#include "test.h"

void test_matrix() {
	std::vector<std::vector<rational>> m_q = { {}, {}, {}, {} };
	matrix m = m_q;

	matrix::matrix_vector v(m.row(3));

	m.print();
	v.print();
	m.row(3)[0] = 5;
	m.print();
	v.print();
	m.push_c(m.col(0).vector());
	m.print();
	v.print();
	m.push_c(m.col(0).vector());
	m.print();
	v.print();
	m.sum_c(m.columns() - 1, m.col(m.columns() - 2).vector());
	m.print();
	v.print();
	m.pop_c();
	v.print();


	m.col(2).print();

	m.push_r(m.row(0).vector());
	m.push_r(m.row(0).vector());
	m.sum_r(m.rows() - 1, m.row(m.rows() - 2).vector());
	m.print();

	m.col(2).print();
}

void test_lp() {
	std::vector<std::vector<rational>> m_q = { {1,2,1,3,10},{2,1,0,1,20},{4,-1,-2,3,40},{4,1,3,2,0} };
	std::vector<int> op_out = { 0, 0, 0 };
	matrix m = m_q;
	lp_problem prob1(m, op_out, 1, 0);
	lp_problem prob = prob1;

	prob.simplex();

	std::cout << "-------DUAL PROBLEM--------- " << std::endl << std::endl;

	prob = prob1.dual();
	prob.simplex();

	std::cout << "-------DUAL-DUAL PROBLEM--------- " << std::endl << std::endl;

	prob = prob1.dual().dual();
	prob.simplex();
}