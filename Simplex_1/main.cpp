#include <iostream>
#include <cassert>
#include <vector>
#include "useful.h"
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"

//#define DEBUG

int main() {

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
	//prob = prob1.dual();

	//std::cout << std::endl << "DUAL PROBLEM" << std::endl;
	//prob.init();
	//prob.print();
	//prob.standard();
	//prob.solve();
	//prob.solution();

	//prob = prob1.dual();
	//prob.init();
	//prob = prob.dual();

	//std::cout << std::endl << "ORIGINAL PROBLEM" << std::endl;
	//prob.init();
	//prob.print();
	//prob.standard();
	//prob.solve();
	//prob.solution();

	//std::cout << "" << std::endl;*/


	return 0;

}