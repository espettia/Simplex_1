#include <iostream>
#include <vector>
#include "useful.h"
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"



//FALTA AGREGAR CONDICIONES PARA OBTENER EL PROBLEMA ESTÁNDAR A PARTIR DEL W PROBLEMA, PUES ES POSIBLE QUE W SE MNINIMICE PERO ENTRE LAS VARIABLES BÁSICAS SIGA HABIENDO ARTIFICIALES




int main() {

	std::vector<std::vector<rational>> m_q = { {1,2,1,3,10},{2,1,0,1,20},{4,-1,-2,3,40},{4,1,3,2,0} };
	std::vector<int> op_out = { 0, 0, 0};
	matrix m = m_q;
	lp_problem prob(m, op_out, 1, 0);

	prob.init();
	prob.standard().print();

	std::cout << "" << std::endl;/*
	prob1.standard();
	prob1.print();
	prob1.solve();
	prob1.solution();*/

	//std::cout << "Dual----------------" << std::endl;
	//prob2.print();
	//prob2.standard();
	//std::cout << "Standard form" << std::endl;
	//prob2.print();
	//prob2.solve();
	//std::cout << "Final form---------------" << std::endl;
	//prob2.solution();

	//std::cout << "Dual again" << std::endl;
	//prob2.dual().standard().solve().solution();

	//std::cout << "Dual again again" << std::endl;
	//prob2.dual().standard().solve().solution();
	//prob2.dual().dual().standard().solve().solution();

	//prob2.dual().dual().dual().standard().solve().solution();


	return 0;

}