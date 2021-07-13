#include <iostream>
#include <vector>
#include "useful.h"
#include "rational.h"
#include "matrix.h"

int main() {

	std::vector<std::vector<rational>> m_q = { {0,2,3,4}, {5,6,7,8} , {9, 10, 11, 12}  };
	matrix m = m_q;

	m.piv(0, 0);
	m.print();

	return 0;

}