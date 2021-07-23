#include <iostream>
#include <cassert>
#include <deque>
#include "useful.h"
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"
#include "lp_algorithm.h"

//#define DEBUG
//test
int main() {

	std::cout << "compiled!" << std::endl;
	std::deque<std::vector<rational>> m_q =
	{ {0, 4, 1, 3, 2},
		{40, 4, -1, -2, 3},
		{20, 2, 1, 0, 1},
		{10, 1, 2, 1, 3} };

	std::deque<int> op_out = { 0, 0, 0, 0, 0};
	std::vector<int> bd_out = { 0, 1, 1, 1, 1 };
	matrix m = m_q;
	lp_problem prob1(m, op_out, bd_out);
	lp_problem prob = prob1;
	simplex::solve(prob);

	std::cout << "-------DUAL PROBLEM--------- " << std::endl << std::endl;

	prob = prob1.dual();
	simplex::solve(prob);

	std::cout << "-------DUAL-DUAL PROBLEM--------- " << std::endl << std::endl;

	prob = prob1.dual().dual();
	simplex::solve(prob);
	return 0;

}