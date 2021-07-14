#include <iostream>
#include <vector>
#include "useful.h"
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"

int main() {

	std::vector<std::vector<rational>> m_q = { {1,-2,-3,-2,3},{1,-1,2,1,11},{2,-3,1,1,0} };
	std::vector<int> op_out = { 1, 1, 1 };
	matrix m = m_q;
	lp_problem prob(m, op_out, 1, 0, 1);

	prob.print();
	prob.dual().print();
	prob.dual().dual().print();


	return 0;

}