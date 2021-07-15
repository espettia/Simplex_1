#include "lp_problem.h"

lp_problem::lp_problem(matrix m_out, std::vector<int> op_out, int type_out, int obj_out, int integer_out, int init_out, int col_out, int row_out, int std, int fin, int nosol, int sol, int noadm, int w, size_t constraints_out, size_t variables_out) {
	m = m_out;
	op = op_out;
	max_type = type_out;
	max_obj = obj_out;
	integer = integer_out;
	st_init = init_out;
	init_col = col_out;
	init_row = row_out;
	st_std = std;
	st_fin = fin;
	st_nosol = nosol;
	st_sol = sol;
	st_noadm = noadm;
	st_w = w;
	constraints = constraints_out;
	variables = variables_out;
}

lp_problem::lp_problem() {
	st_init = 0;
}

lp_problem lp_problem:: operator=(lp_problem p) {

	m = p.m;
	op = p.op;
	max_type = p.max_type;
	max_obj = p.max_obj;
	integer = p.integer;
	st_init = p.st_init;
	init_col = p.init_col;
	init_row = p.init_row;
	st_std = p.st_std;
	st_fin = p.st_fin;
	st_nosol = p.st_nosol;
	st_sol = p.st_sol;
	st_noadm = p.st_noadm;
	st_w = p.st_w;
	constraints = p.constraints;
	variables = p.variables;

	return *this;
}

std::vector<rational>& lp_problem::objective_function() {
	if (!st_init)
		exit(1);
	return m.last_r();
}

void lp_problem::save_objective_function() {
	if (st_saved_objective_function)
	{
		std::cout << "There is one objective function already saved";
		exit(30);
	}
	saved_objective_function = objective_function();
	st_saved_objective_function = 1;
	m.pop_r();
}

void lp_problem::restore_objective_function() {
	if (!st_saved_objective_function)
	{
		std::cout << "There is no objective function to be restored";
		exit(30);
	}
	m.push_r(saved_objective_function);
	st_saved_objective_function = 0;
}

std::vector<rational> lp_problem::constant_terms() {
	if (!st_init)
		exit(1);
	return m.last_c();
}

void lp_problem::save_constant_terms() {
	if (st_saved_constant_terms)
	{
		std::cout << "There is one column of constant terms already saved";
		exit(31);
	}
	saved_constant_terms = m.last_c();
	st_saved_constant_terms = 1;
	m.pop_c();
}

void lp_problem::restore_constant_terms() {
	if (!st_saved_constant_terms)
	{
		std::cout << "There are no constant terms to be restored";
		exit(32);
	}
	m.push_c(saved_constant_terms);
	st_saved_constant_terms = 0;
}


std::vector<int> lp_problem::basic_var() {
	for (size_t i = 0; i < (size_t)constraints; ++i) {
		for (size_t j = 0; j < (size_t)variables; ++j) {
			if (m.at(i, j) == 1 && is_canonical(m.col(j))) {
				basic[i] = j; break;
			}
		}
	}
	return basic;
}

bool lp_problem::is_canonical(std::vector<rational> v) {
	int flag = 0;
	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != 0) {
			if (v[i] == 1 && flag == 0) flag = 1;
			else return 0;
		}
	}
	return 1;
}

bool lp_problem::init() {
	st_init = false;

	init_col = m.columns();
	init_row = m.rows();
	constraints = st_w ? init_row - 2 : init_row - 1;
	variables = init_col - 1;

	if (m.rows() < 2 || (op.size() != constraints) || m.columns() < 2 || constraints < 1) {
		std::cout << "Not valid parameters";
		exit(10);
	}

	std::vector<int> basic_(constraints, -1);
	basic = basic_;
	st_init = true;
	return st_init;
}
//
//void lp_problem::check_status(int operation) {

	//1 - to min prob
	//2 - to max prob
	//3 - to min obj
	//4 - add slack
	//5 - add artificial
	//6 - remove artificial
	//7 - solve w problem
	//8 - solve z problem
	//9 - dual
	//10 - solution
	//11 - basic var
	//12 - find pivot
	//13 - save obj
	//14 - restore obj
	//15 - save const
	//16 - restore const


	//std::cout << "" << endl;
//}

//TO EXIT CODE ADD 100

////0
//void lp_problem::need_max_type() {
//	if (!max_type) {
//		std::cout << "Problem need to be in Max type" << std::endl;
//		exit(100);
//	}
//}
//
//void lp_problem::need_min_type() {
//	if (max_type) {
//		std::cout << "Problem need to be in Min type" << std::endl;
//		exit(101);
//	}
//}
//
//void lp_problem::need_max_obj() {
//	if (!max_obj) {
//		std::cout << "Objective function need to be in Max form" << std::endl;
//		exit(102);
//	}
//}
//
//void lp_problem::need_min_obj() {
//	if (max_obj) {
//		std::cout << "Objective function need to be in min form" << std::endl;
//		exit(103);
//	}
//}
//
//void lp_problem::need_integer() {
//	if (!integer) {
//		std::cout << "Problem need to be integer LPP" << std::endl;
//		exit(103);
//	}
//}
//
//void lp_problem::need_real() {
//	if (integer) {
//		std::cout << "Problem need to be real LPP" << std::endl;
//		exit(103);
//	}
//}
//
//void lp_problem::need_init() {
//	if (!integer) {
//		std::cout << "Problem need to be integer LPP" << std::endl;
//		exit(103);
//	}
//}
////int st_init = 0;//
////size_t init_col = 0;
////size_t init_row = 0;
////int st_std = 0;
////int st_fin = 0;
////int st_nosol = 0;
////int st_sol = 0;
////int st_noadm = 0;
////int st_w = 0;

bool lp_problem::check_dual_simplex() {
	for (size_t i = 0; i < variables; ++i)
		if (objective_function()[i] < 0) return 0;
	return 1;
}

bool lp_problem::check_solved() {

	for (size_t c = 0; c < variables; ++c) {
		if (objective_function()[c] < 0) {
			st_sol = 0;  return 0;
		}
	}

	st_sol = 1;
	return 1;
}

bool lp_problem::check_unbounded() {

	//last column are the constant terms
	for (size_t j = 0; j < variables; ++j) {
		if (objective_function()[j] < 0) {
			bool exists_positive = 0;
			int negative_counter = 0;
			//last row is the objective function
			for (size_t i = 0; i < constraints; ++i) {
				if (m.at(i, j) > 0) {
					exists_positive = true; break;
				}
			}
			if (!exists_positive) {
				st_nosol = 1;  return 1;
			}
		}
	}

	st_nosol = 0;
	return 0;
}

bool lp_problem::check_finished() {
	st_fin = check_solved() || check_unbounded();
	return st_fin;
}

int lp_problem::check_w_valid() {
	//Assuming system is in canonical form
	int valid_basic_variables_counter = 0;
	for (auto& i : basic) {
		if ((size_t)basic[i] < variables - artificial_variables) ++valid_basic_variables_counter;
	}
	return valid_basic_variables_counter;
}

lp_problem lp_problem::to_min_obj() {
	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}
	//Multiply objctive function by -1 if it is a max problem
	if (max_obj) {
		objective_function() = rational(-1) * objective_function();
		max_obj = 0;
	}
	return *this;
}

lp_problem lp_problem::to_min_problem() {

	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}

	//Multiply objctive function by -1
	if (max_obj) {
		objective_function() = rational(-1) * objective_function();
		max_obj = 0;
	}

	save_objective_function();

	//Add or modify equations to obtain >= form
	for (size_t i = 0; i < constraints; ++i) {

		if (op[i] == 0) {//=

			//Create >= <= equations to replace =

			//Create <= at the bottom
			op.push_back(1);//1 stands for <=
			m.push_r(m.row(i));
			++constraints;
			basic.push_back(-1);

			//Create => (just change the operation)
			op[i] = 2;//2 stands for =>
		}
		else if (op[i] == 1) {//<=
			//Multiply by -1 to obtain >= equation
			m.mult_r(i, -1);
			op[i] = 2;//>=
		}

	}

	restore_objective_function();

	return *this;
}

lp_problem lp_problem::add_slack_variables() {

	save_constant_terms();

	//last equation is the objective function
	for (size_t i = 0; i < m.rows() - 1; ++i) {
		if (op[i] == 2) {//>=
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = -1;
			m.push_c(slack);
			++variables;
		}
		else if (op[i] == 1) {//<=
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = 1;
			m.push_c(slack);
			++variables;
		}
		op[i] = 0;//=
	}

	restore_constant_terms();

	return *this;
}

lp_problem lp_problem::make_coefficients_positive() {
	std::vector<rational> temp_constants = constant_terms();

	//Last equation is the objective function
	for (size_t i = 0; i < constraints; ++i) {
		if (temp_constants[i] < 0) {
			m.mult_r(i, -1);
		}
	}
	return *this;
}

lp_problem lp_problem::add_artificial_variables() {

	//Store constant terms to add variables to the right
	save_constant_terms();

	//Add a number of variables equal to the number of constraints
	for (size_t i = 0; i < constraints; ++i) {
		std::vector<rational> artificial(m.rows(), 0);
		artificial[i] = 1;
		m.push_c(artificial);
		++variables;
		++artificial_variables;
	}

	restore_constant_terms();

	//Add w-problem

	std::vector<rational> w_problem(m.columns(), 0);
	for (size_t i = 0; i < constraints; ++i) {
		for (size_t j = 0; j < variables - artificial_variables; ++j) {
			w_problem[j] = w_problem[j] - m.at(i, j);
		}
		w_problem[variables] = w_problem[variables] - m.at(i, variables);
	}

	//There's a new objective function
	m.push_r(w_problem);
	op.push_back(0);
	return *this;
}

lp_problem lp_problem::remove_artificial_variables() {

	//Remove w-problem
	m.pop_r();
	op.pop_back();

	//Remove variables
	save_constant_terms();

	for (size_t j = 0; j < constraints; ++j) {
		m.pop_c();
		--variables;
		--artificial_variables;
	}

	restore_constant_terms();
	return *this;
}

lp_problem lp_problem::standard() {


	st_noadm = 0;

	//CHECK VALID PROBLEM
	if (!st_init) {
		std::cout << "Problem not initialized" << std::endl;
		exit(12);
	}

	//PERFORM OPERATIONS
	to_min_obj();
	add_slack_variables();
	make_coefficients_positive();
	add_artificial_variables();

	//UPDATE STATUS
	st_std = 1;
	st_w = 1;
	st_fin = 0;

	//PERFORM OPERATIONS
	solve();

	//END OF OPERATIONS
	remove_artificial_variables();
	st_w = 0;
	st_sol = 0;
	st_nosol = 0;

	return *this;
}

std::vector<size_t> lp_problem::simplex_find_pivot() {

	rational ck_gr;
	rational ck;
	rational aik_lw;
	rational aik;
	size_t i_lw;
	size_t k_gr;
	int flag;

	flag = 0;
	for (size_t k = 0; k < variables; ++k) {
		ck = m.at(m.rows() - 1, k);
		if (ck < 0) {
			ck = ck * ck.sign();
			if (flag == 0) {
				ck_gr = ck;
				k_gr = k;
				flag = 1;
			}
			else if (ck > ck_gr) {
				ck_gr = ck;
				k_gr = k;
			}
		}
	}
	flag = 0;
	for (size_t i = 0; i < constraints; ++i) {
		aik = m.at(i, k_gr);
		if (aik > 0) {
			aik = m.at(i, m.columns() - 1) / aik;
			if (flag == 0) {
				aik_lw = aik;
				i_lw = i;
				flag = 1;
			}
			else if (aik < aik_lw) {
				aik_lw = aik;
				i_lw = i;
			}
		}
	}

	std::vector<size_t> pivot(2);
	pivot[0] = i_lw;
	pivot[1] = k_gr;

	if (m.at(i_lw, k_gr) == 0)
		int a = 0;

	return pivot;
}

lp_problem lp_problem::solve() {

	if (!st_std) {
		std::cout << "LP-Problem need to be in standard form"; exit(6);
	}

	std::vector<size_t> pivot;

	while (check_finished(), !st_fin) {

		pivot = simplex_find_pivot();
		//For debugging
#ifdef DEBUG
		std::cout << "pick:" << pivot[0] << " " << pivot[1] << std::endl;
#endif // DEBUG

		m.piv(pivot[0], pivot[1]);
		basic[pivot[0]] = pivot[1];
		if (pivot[0] == 3 && pivot[1] == 6)
			int a = 1;

#ifdef DEBUG
		print();
#endif // DEBUG
	}

	if (st_w) {

		//CHECK IF STANDARD FORM HAS BEEN REACHED
		if (objective_function()[variables] == 0) {
			st_std = 1; st_noadm = 0; st_fin = 0;
		}
		else {
			st_noadm = 1; st_std = 0; st_fin = 1;
			std::cout << "The problem does not have any feasible solutions";
			exit(1000);
		}

		std::vector<int> basic_map(variables, 0);
		size_t valid_basic_variables = 0;
		for (size_t j = 0; j < variables; ++j) {
			if (is_canonical(m.col(j))) {
				basic_map[j] = 1;
				if (j < variables - artificial_variables) ++valid_basic_variables;
			}
		}

		if (valid_basic_variables == constraints) {
			basic_var();
		}
		else
		{
			size_t missing_basic_variables = constraints - valid_basic_variables;
			for (size_t i = 0; i < constraints; ++i) {
				if ((size_t)basic[i] >= variables - artificial_variables) {
					for (size_t j = 0; j < variables; ++j) {
						if (basic_map[j] == 0 && m.at(i, j) != 0) {
							//For debugging
#ifdef DEBUG
							std::cout << "Wpick:" << i << " " << j << std::endl;
#endif // DEBUG
							m.piv(i, j);
#ifdef DEBUG
							print();
#endif // DEBUG
							basic[i] = (int)j;
							basic_map[j] = 1;
							--missing_basic_variables;
							break;
						}
					}
					if (missing_basic_variables == 0) break;
				}
			}
			if (missing_basic_variables != 0) {
				save_objective_function();
				std::vector<rational> z_function = m.last_r();
				m.pop_r();
				for (size_t i = 0; i < missing_basic_variables; ++i) {
					m.pop_r();
				}
				m.push_r(z_function);
				restore_objective_function();
			}
		}
	}




	return *this;
}


//int lp_problem::solve_dual() {
//
//	rational ck_gr;
//	rational ck;
//	rational aik_lw;
//	rational aik;
//	size_t i_lw;
//	size_t k_gr;
//	int flag;
//
//	if (!check_dual_simplex()) {
//	std::cout << "Not suitable for Dual-simplex";
//	exit(11);
//	}
//
//	while (st_fin = this->check_finished(), !st_fin) {
//
//		flag = 0;
//		for (size_t k = 0; k < m.columns() - 1; ++k) {
//			ck = m.at(m.rows() - 1, k);
//			if (ck < 0) {
//				ck = ck * ck.sign();
//				if (flag == 0) {
//					ck_gr = ck;
//					k_gr = k;
//					flag = 1;
//				}
//				else if (ck > ck_gr) {
//					ck_gr = ck;
//					k_gr = k;
//				}
//			}
//		}
//		flag = 0;
//		for (size_t i = 0; i < m.rows() - 1 - !!z_w; ++i) {
//			aik = m.at(i, k_gr);
//			if (aik > 0) {
//				aik = m.at(i, m.columns() - 1) / aik;
//				if (flag == 0) {
//					aik_lw = aik;
//					i_lw = i;
//					flag = 1;
//				}
//				else if (aik < aik_lw) {
//					aik_lw = aik;
//					i_lw = i;
//				}
//			}
//		}
//
//		std::cout << "pick:" << i_lw << " " << k_gr << std::endl;
//		m.piv(i_lw, k_gr);
//		this->print();
//	}
//	return st_fin;
//}



lp_problem lp_problem::to_max_problem() {

	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}

	//Multiply objctive function by -1
	if (!max_obj) {
		objective_function() = rational(-1) * objective_function();
		max_obj = 0;
	}

	save_objective_function();

	//Add or modify equations to obtain >= form
	for (size_t i = 0; i < constraints; ++i) {

		if (op[i] == 0) {//=

			//Create >= <= equations to replace =

			//Create => at the bottom
			op.push_back(2);//1 stands for =>
			m.push_r(m.row(i));
			++constraints;
			basic.push_back(-1);

			//Create <= (just change the operation)
			op[i] = 1;//1 stands for <=
		}
		else if (op[i] == 2) {//>=
			//Multiply by -1 to obtain <= equation
			m.mult_r(i, -1);
			op[i] = 1;//<=
		}

	}

	restore_objective_function();

	return *this;
}

lp_problem lp_problem::dual() {

	lp_problem p1 = *this;

	p1.st_std = 0;
	p1.st_fin = 0;
	p1.st_nosol = 0;
	p1.st_sol = 0;
	p1.st_noadm = 0;
	p1.st_w = 0;

	p1.to_max_problem();

	p1.m = p1.m.transpose();
	int temp = p1.constraints;
	p1.constraints = p1.variables;
	p1.variables = temp;
	p1.max_obj = 0;
	p1.max_type = !p1.max_type;
	p1.op = std::vector<int>(p1.m.rows() - 1, 2);

	p1.st_std = 0;
	p1.st_fin = 0;
	p1.st_nosol = 0;
	p1.st_sol = 0;
	p1.st_noadm = 0;
	p1.st_w = 0;
	p1.st_init = 0;

	return p1;
}

std::vector<rational> lp_problem::solution() {

	if (!st_fin) { std::cout << "Problem need to be in final form to provide a solution"; exit(7); }

	if (st_noadm) {
		std::cout << "System does not have any feasible basic solution"; exit(8);
	}

	if (st_nosol) {
		std::cout << "Problem is unbounded"; exit(9);
	}

	if (!st_sol) {
		std::cout << "Problem nee dto be solved"; exit(13);
	}

	std::cout << "LP-Problem has an optimal solution at (";

	std::vector<rational> sol(init_col - 1, 0);
	std::vector<rational> constants = constant_terms();

	//FIND VALUES OF SOLUTION
	for (size_t c = 0; c < init_row - 1; ++c) {
		if ((size_t)basic[c] < init_col - 1) {
			sol[basic[c]] = constants[c];
		}
	}

	//pRINT SOLUTION
	int flag = 0;
	for (size_t c = 0; c < init_col - 1; ++c) {
		if (flag == 1)
			std::cout << ", ";
		sol[c].print(1);
		flag = 1;
	}

	std::cout << ")" << std::endl;
	std::cout << "Optimal value is ";
	rational optimal = (max_type ? m.at(m.rows() - 1, m.columns() - 1) : m.at(m.rows() - 1, m.columns() - 1) * -1);
	optimal.print();
	std::cout << std::endl;
	return sol;
}

// Done


void lp_problem::print_op(size_t i) {
	switch (op[i]) {
	case 1: std::cout << "<=";
		break;
	case 2: std::cout << ">=";
		break;
	default: std::cout << "=";
	}
}

void lp_problem::print() {
	if (st_init) {

		size_t last = m.columns() - 1;

		//Print objective function
		std::vector<rational> function = objective_function();
		std::cout << (max_obj ? "Maximize  " : "Minimize  ");
		int flag = 0;
		for (size_t c = 0; c < last; ++c) {
			if (function[c].sign() == 1 && flag == 1) std::cout << " + ";
			function[c].print();
			if (function[c].sign() != 0) {
				std::cout << "X" << c << "  "; flag = 1;
			}
		}
		std::cout << " = ";

		function[last].print(1);
		std::cout << (max_type == max_obj ? " + z " : " - z ");
		std::cout << std::endl;

		//Print constraints
		std::cout << "Subject to:" << std::endl;
		for (size_t i = 0; i < m.rows() - 1; ++i) {
			flag = 0;
			for (size_t j = 0; j < last; ++j) {
				if (m.at(i, j).num() != 0) {
					if (m.at(i, j).sign() == 1 && flag == 1) {
						std::cout << " + ";
					}
					m.at(i, j).print();
					if (m.at(i, j).sign() != 0) std::cout << "X" << j << "  ";
					flag = 1;
				}
				else {
					std::cout << "       ";
				}
			}
			print_op(i);
			std::cout << "  ";
			m.at(i, last).print(1);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else
		std::cout << "Not initialized" << std::endl;
}

