#include "lp_problem.h"

constexpr int LEQ = -1;
constexpr int GEQ = 1;
constexpr int EQ = 0;

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

	init();

}

matrix::matrix_vector lp_problem::objective_function() {

	if (!st_init)
	{
		std::cerr << "Problem need to be initialized";
		exit(1);
	}

	return m.last_r();
}

void lp_problem::save_objective_function() {

	assert(!st_saved_objective_function);

	saved_objective_function = objective_function().vector();
	st_saved_objective_function = 1;
	m.pop_r();
}

void lp_problem::restore_objective_function() {
	
	assert(st_saved_objective_function);

	m.push_r(saved_objective_function);
	st_saved_objective_function = 0;
}

matrix::matrix_vector lp_problem::constant_terms() {

	assert(!st_saved_constant_terms);

	if (!st_init)
	{
		std::cerr << "Problem need to be initialized";
		exit(1);
	}

	return m.last_c();
}

void lp_problem::save_constant_terms() {

	assert(!st_saved_constant_terms);

	saved_constant_terms = m.last_c().vector();
	st_saved_constant_terms = 1;
	m.pop_c();
}

void lp_problem::restore_constant_terms() {

	assert(st_saved_constant_terms);

	m.push_c(saved_constant_terms);
	st_saved_constant_terms = 0;
}

void lp_problem::mult_eq(size_t i, rational q) {

	assert(st_init);

	m.mult_r(i, q);

	if (op[i] > GEQ) op[i] = LEQ;
	else if (op[i] < LEQ) op[i] = GEQ;
}

std::vector<int> lp_problem::basic_var() {

	if (!st_init)
	{
		std::cerr << "Problem need to be initialized";
		exit(1);
	}

	assert(st_std);

	for (size_t i = 0; i < constraints; ++i) {
		for (size_t j = 0; j < variables; ++j) {
			if (m.at(i, j) == 1 && is_canonical(m.col(j).vector())) {
				basic[i] = j; break;
			}
		}
	}

	return basic;
}

lp_problem lp_problem::init() {
#ifndef NDEBUG
	std::cout << "init()" << std::endl;
#endif // !NDEBUG
	st_init = false;
	init_col = m.columns();
	init_row = m.rows();
	constraints = st_w ? init_row - 2 : init_row - 1;
	variables = init_col - 1;

	if (init_row < 2 || op.size() != constraints || init_col < 2 || constraints < 1 || variables < 1) {
		std::cout << "Not valid parameters";
		exit(10);
	}

	basic = std::vector<int>(constraints, -1);
	st_init = true;

#ifndef NDEBUG
	print();
	std::cout << "DONE!" << std::endl;
#endif // !NDEBUG
	return *this;
}

//bool lp_problem::check_dual_simplex() {
//	//FALTAN MÁS CONDICIONES
//	for (size_t i = 0; i < variables; ++i)
//		if (objective_function()[i] < 0) return 0;
//	return 1;
//}

bool lp_problem::check_solved() {

	assert(st_std);

	for (size_t i = 0; i < variables; ++i) {
		if (objective_function()[i] < 0) {
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
		objective_function() = rational(-1) * objective_function().vector();
		max_obj = 0;
	}
	return *this;
}

lp_problem lp_problem::to_max_obj() {
	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}
	//Multiply objctive function by -1 if it is a max problem
	if (!max_obj) {
		objective_function() = rational(-1) * objective_function().vector();
		max_obj = 0;
	}
	return *this;
}

lp_problem lp_problem::to_min_problem() {

	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}

	to_min_obj();

	save_objective_function();

	//Add or modify equations to obtain >= form
	for (size_t i = 0; i < constraints; ++i) {

		if (op[i] == EQ) {
			//Create >= <= equations to replace =

			//Create <= at the bottom
			op.push_back(1);//1 stands for <=
			m.push_r(m.row(i).vector());
			++constraints;
			basic.push_back(-1);

			op[i] = GEQ;
		}
		else if (op[i] == GEQ) {//<=
			mult_eq(i, -1);
		}

	}

	restore_objective_function();

	return *this;
}

lp_problem lp_problem::add_slack_variables() {

	save_constant_terms();

	//last equation is the objective function
	for (size_t i = 0; i < m.rows() - 1; ++i) {
		if (op[i] == GEQ) {//>=
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = -1;
			m.push_c(slack);
			++variables;
		}
		else if (op[i] == LEQ) {//<=
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = 1;
			m.push_c(slack);
			++variables;
		}
		op[i] = EQ;//=
	}

	restore_constant_terms();

	return *this;
}

lp_problem lp_problem::make_constants_positive() {

	std::vector<rational> temp_constants = constant_terms().vector();

	//Last equation is the objective function
	for (size_t i = 0; i < constraints; ++i) {
		if (temp_constants[i] < 0) {
			mult_eq(i, -1);
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
	op.push_back(EQ);

	st_w = 1;

	return *this;
}

lp_problem lp_problem::remove_artificial_variables() {

	assert(st_w);

	//Remove w-problem
	m.pop_r();
	op.pop_back();

	//Remove variables
	save_constant_terms();

	while(artificial_variables) {
		m.pop_c();
		--variables;
		--artificial_variables;
	}

	st_w = 0;
	st_fin = 0;


	restore_constant_terms();

	return *this;
}

lp_problem lp_problem::standard() {
#ifndef NDEBUG
	std::cout << "standard()" << std::endl;
#endif // !NDEBUG

	st_noadm = 0;

	//CHECK VALID PROBLEM
	if (!st_init) {
		std::cout << "Problem not initialized" << std::endl;
		exit(12);
	}

	//PERFORM OPERATIONS
	to_min_obj();

#ifndef NDEBUG
	std::cout << "Objective function is now min"<<std::endl;
	print();
#endif // !NDEBUG

	add_slack_variables();

#ifndef NDEBUG
	std::cout << "Slack variables added" << std::endl;
	print();
#endif // !NDEBUG

	make_constants_positive();

#ifndef NDEBUG
	std::cout << "Constants made positive" << std::endl;
	print();
#endif // !NDEBUG

	add_artificial_variables();

#ifndef NDEBUG
	std::cout << "Artificial variables and w-problem added" << std::endl;
	print();
#endif // !NDEBUG

	//UPDATE STATUS
	st_std = 1;
	st_w = 1;
	st_fin = 0;

#ifndef NDEBUG
	std::cout << "Proceeding to solve W-problem to find canonical form" << std::endl;
	print();
#endif // !NDEBUG

	//PERFORM OPERATIONS
	solve();

	//END OF OPERATIONS
	remove_artificial_variables();

#ifndef NDEBUG
	std::cout << "Artificial variables and w-problem removed" << std::endl;
	print();
#endif // !NDEBUG

	st_w = 0;
	st_sol = 0;
	st_nosol = 0;
	st_noadm = 0;
	st_fin = 0;

#ifndef NDEBUG
	std::cout << "standard() DONE!" << std::endl;
#endif // !NDEBUG

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

#ifndef NDEBUG
	std::cout << "solve()" << std::endl;
#endif // !NDEBUG

	if (!st_std) {
		std::cout << "LP-Problem need to be in standard form"; exit(6);
	}

	std::vector<size_t> pivot;

	while (check_finished(), !st_fin) {

		pivot = simplex_find_pivot();

		//For debugging

#ifndef NDEBUG
		std::cout << std::endl << "Pivot at:" << pivot[0] << " " << pivot[1] << std::endl;
#endif // NDEBUG

		m.piv(pivot[0], pivot[1]);
		basic[pivot[0]] = pivot[1];

#ifndef NDEBUG
		print();
#endif // NDEBUG
	}

	if (st_w) {

		//CHECK IF STANDARD FORM HAS BEEN REACHED
		if (objective_function()[variables] == 0) {
			st_std = 1;
		}
		else {
			st_noadm = 1; st_std = 0;
			std::cout << "LP-Problem does not have any feasible solutions";
			exit(1000);
		}

		std::vector<int> basic_map(variables, 0);
		size_t valid_basic_variables = 0;
		for (size_t j = 0; j < variables; ++j) {
			if (is_canonical(m.col(j).vector())) {
				basic_map[j] = 1;
				if (j < variables - artificial_variables) ++valid_basic_variables;
			}
		}

#ifndef NDEBUG
		std::cout << "Map of basic variables: " << std::endl;
		for (auto& o : basic_map) std::cout << o << " ";
		std::cout << std::endl;
#endif // !NDEBUG

		if (valid_basic_variables == variables - artificial_variables) {

			basic_var();

#ifndef NDEBUG
			std::cout << "STANDARD FORM FOUND. Basic variables are: " << std::endl;
			for (auto& o : basic) std::cout << o << " ";
			std::cout << std::endl;
#endif // !NDEBUG

		}
		else
		{

#ifndef NDEBUG
			std::cout << "W-PROBLEM SOLVED. STANDARD FORM NOT FOUND" << std::endl << "PROCEED TO REPLACE ARTIFICIAL VARIABLES WITH ORIGINALS"<<std::endl;
#endif // !NDEBUG

			size_t missing_basic_variables = constraints - valid_basic_variables;
			for (size_t i = 0; i < constraints; ++i) {
				if ((size_t)basic[i] >= variables - artificial_variables) {
					for (size_t j = 0; j < variables; ++j) {
						if (basic_map[j] == 0 && m.at(i, j) != 0) {
							//For debugging
#ifndef NDEBUG
							std::cout << "Pivot at:" << i << " " << j << std::endl;
#endif // NDEBUG
							m.piv(i, j);
#ifndef NDEBUG
							print();
#endif // NDEBUG
							basic[i] = (int)j;
							basic_map[j] = 1;

#ifndef NDEBUG
							std::cout << "Now, basic variables are: ";
							for (auto& o : basic) std::cout << o << " ";
							std::cout << std::endl;
#endif // !NDEBUG
							--missing_basic_variables;
							break;
						}
					}
					if (missing_basic_variables == 0) break;
				}
			}
			if (missing_basic_variables != 0) {
#ifndef NDEBUG
				std::cout << "REMOVING REDUNDANT EQUATIONS"<< std::endl;
#endif // !NDEBUG

				save_objective_function();
				std::vector<rational> z_function = m.last_r().vector();
				m.pop_r();
				for (size_t i = 0; i < missing_basic_variables; ++i) {
					m.pop_r();
#ifndef NDEBUG
					std::cout << "REMOVED " << i + 1 << (i == 0 ? " EQUATION" : "EQUATIONS") << std::endl;
#endif // !NDEBUG
				}
				m.push_r(z_function);
				restore_objective_function();
#ifndef NDEBUG
				std::cout << "PROBLEM BECOMES:" << std::endl;
				print();
#endif // !NDEBUG
			}
			
#ifndef NDEBUG
			std::cout << "W-PROBLEM SOLVED. STANDARD FORM HAS BEEN FOUND." << std::endl;
#endif // !NDEBUG
		}

	}

#ifndef NDEBUG
		std::cout << "solve() DONE!" << std::endl;
#endif // !NDEBUG


	return *this;
}

lp_problem lp_problem::to_max_problem() {

	if (!st_init) {
		std::cout << "Problem need to be initialized" << std::endl;
		exit(11);
	}

	to_max_obj();

	save_objective_function();

	//Add or modify equations to obtain >= form
	for (size_t i = 0; i < constraints; ++i) {

		if (op[i] == EQ) {//=

			//Create >= <= equations to replace =

			//Create => at the bottom
			op.push_back(GEQ);//1 stands for =>
			m.push_r(m.row(i).vector());
			++constraints;
			basic.push_back(-1);

			//Create <= (just change the operation)
			op[i] = LEQ;//1 stands for <=
		}
		else if (op[i] == GEQ) {//>=
			//Multiply by -1 to obtain <= equation
			mult_eq(i, -1);
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

	p1.op = std::vector<int>(p1.m.rows() - 1, GEQ);

	size_t temp_constraints = p1.constraints;
	p1.constraints = p1.variables;
	p1.variables = temp_constraints;
	p1.max_obj = 0;
	p1.max_type = !p1.max_type;
	
	p1.st_std = 0;
	p1.st_fin = 0;
	p1.st_nosol = 0;
	p1.st_sol = 0;
	p1.st_noadm = 0;
	p1.st_w = 0;
	p1.st_init = 0;

	p1.init();

	return p1;
}

lp_problem lp_problem::simplex() {

	assert(st_init);

	standard();
	solve();
	solution();

	return *this;

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
	std::vector<rational> constants = constant_terms().vector();

	//FIND VALUES OF SOLUTION
	//ASUMMING ALL EQUATIONS HAVE VALID BASIC VARIABLES
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
	if (op[i] < 0) std::cout << "<=";
	else if (op[i] > 0) std::cout << ">=";
	else std::cout << "=";
}

void lp_problem::print() {
	if (st_init) {

		size_t last = m.columns() - 1;

		//Print objective function
		std::vector<rational> function = objective_function().vector();
		std::cout << (max_obj ? "Maximize  " : "Minimize  ");
		int flag = 0;
		for (size_t c = 0; c < last; ++c) {
			if (function[c].sign() == 1 && flag == 1) std::cout << " +";
			function[c].print();
			if (function[c].sign() != 0) {
				std::cout << "X" << c << "  "; flag = 1;
			}
		}
		std::cout << " = ";

		function[last].print(1);
		std::cout << (st_w? " + w " : (max_type == max_obj ? " + z " : " - z "));
		std::cout << std::endl;

		////Print objective function
		//std::vector<rational> function = objective_function();
		//std::cout << (max_obj ? "Maximize  " : "Minimize  ");
		//int flag = 0;
		//for (size_t c = 0; c < last; ++c) {
		//	std::string term;
		//	if (function[c].sign() == 1 && flag == 1) term.append(" + ");
		//	term.append(function[c].str());
		//	if (function[c].sign() != 0) {
		//		//i don't know if it works
		//		term.append("X" + std::to_string(c)); flag = 1;
		//	}
		//	std::cout << std::setw(10) << term;
		//}

		//std::cout << " = ";

		//std::cout << std::setw(6) << function[last].str(0x01);
		//std::cout << (st_w ? " + w " : (max_type == max_obj ? " + z " : " - z "));
		//std::cout << std::endl;

		//Print constraints
		std::cout << "Subject to:" << std::endl;
		for (size_t i = 0; i < m.rows() - 1; ++i) {
			flag = 0;
			for (size_t j = 0; j < last; ++j) {
				std::string term;
				if (m.at(i, j).num() != 0) {
					if (m.at(i, j).sign() == 1 && flag == 1) 
						term.append("+");
					term.append(m.at(i, j).str());
					if (m.at(i, j).sign() != 0) 
						term.append("X" + std::to_string(j));
					flag = 1;
				}
				std::cout << std::setw(10) << term;
			}
			std::cout << " ";
			print_op(i);
			std::cout << " ";
			m.at(i, last).print(1);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else
		std::cout << "Not initialized" << std::endl;
}

