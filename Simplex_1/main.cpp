#include <iostream>
#include <vector>
#include "useful.h"
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"

//#define DEBUG

int main() {

	rational a(-1, 3);
	a.frac().print();

	/*std::vector<std::vector<rational>> m_q = { {8,-1,21},{1,-6,13},{3,-4,-11},{12,2,0} };
	std::vector<int> op_out = { 1, 2, 0 };
	matrix m = m_q;
	lp_problem prob(m, op_out, 1, 1);
	lp_problem prob1 = prob;

	prob1.init();

	prob.init();
	prob.print();
	prob.standard();
	prob.solve();
	prob.solution();

	prob = prob1.dual();

	std::cout << std::endl << "DUAL PROBLEM" << std::endl;
	prob.init();
	prob.print();
	prob.standard();
	prob.solve();
	prob.solution();

	prob = prob1.dual();
	prob.init();
	prob = prob.dual();

	std::cout << std::endl << "ORIGINAL PROBLEM" << std::endl;
	prob.init();
	prob.print();
	prob.standard();
	prob.solve();
	prob.solution();

	std::cout << "" << std::endl;*/


	return 0;

}