#include "lp_algorithm.h"

///___________________________________________________________________________________________///
///___________________________________________________________________________________________///
///_________________________________CLASS: SIMPLEX BASE_______________________________________///
///___________________________________________________________________________________________///
///___________________________________________________________________________________________///

bool simplex_base::check_solvable(lp_problem p_out) {
	for (auto& it : p_out.constant_terms_only_constraints())
		if (it < 0) return 0;
	for (auto& it : p_out.boundaries())
		if (it <= 0) return 0;
	//Check if system is canonical

	return 1;
}

bool simplex_base::check_solved() {
	for (const auto& term : p.objective_function_only_variables())
		if (term < 0)
			return 0;
	return 1;
}

bool simplex_base::check_unbounded() {

	size_t j = 1;
	for (const auto& it : p.objective_function_only_variables()) {
		if (it < 0) {
			bool exists_positive = 0;
			//last row is the objective function
			for (const auto& it2 : p.column_only_constraints(j)) {
				if (it2 > 0) {
					exists_positive = true; break;
				}
			}
			if (!exists_positive)
				return 1;
		}
		++j;
	}
	return 0;
}

bool	simplex_base::check_finished() {
	return check_solved() || check_unbounded();
}

std::vector<size_t> simplex_base::find_pivot() {

	rational ck_lw;
	rational ck;
	rational aik_lw;
	rational aik;
	size_t i_lw;
	size_t k_lw;
	int flag;

	flag = 0;
	size_t k = 1;
	for (const auto& ck : p.objective_function_only_variables()) {
		if (ck < 0) {
			if (flag == 0) {
				ck_lw = ck;
				k_lw = k;
				flag = 1;
			}
			else if (ck < ck_lw) {
				ck_lw = ck;
				k_lw = k;
			}
		}
		++k;
	}

	flag = 0;
	size_t i = p.number_of_obj_functions();

	//Usé esta forma para simplex debería hacerla universal para simplex_base ybsimplex al mismo timeop
	//de alguna forma debe ignorar la última ecuación cuando pivotea simplex
	//pero simplex_base pivotea sobre todas las ecuaciones

	//NO está mal pivotear, si no encontrar el pivote
	for(auto aik : p.column_only_constraints(k_lw)){
		if (aik > 0) {
			aik = p.constant_terms()[i] / aik;
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
		++i;
	}

	std::vector<size_t> pivot(2);
	pivot[0] = i_lw;
	pivot[1] = k_lw;

	return pivot;
}



solution simplex_base::solve(lp_problem p_out, std::vector<int>basic_var_out) {

	simplex_base problem(p_out);
	problem.basic_var_in = basic_var_out;

#ifndef NDEBUG
	std::cout << "solve()" << std::endl;
	problem.p.print_tableau();
#endif // !NDEBUG

	std::vector<size_t> pivot;
	while (!problem.check_finished()) {
		pivot = problem.find_pivot();
#ifndef NDEBUG
		std::cout << std::endl << "Pivot at: " << pivot[0] - problem.p.number_of_obj_functions() << " " << pivot[1] - 1 << std::endl << std::endl;
#endif // NDEBUG
		problem.p.piv(pivot[0], pivot[1]);
		problem.basic_var_in[pivot[0] - problem.p.number_of_obj_functions()] = pivot[1];
#ifndef NDEBUG
		problem.p.print_tableau();
		for (auto& asdf : problem.basic_var_in) std::cout << asdf << " ";
		std::cout << std::endl;
#endif // NDEBUG
	}

	return solution(problem.p_original, problem.p, problem.basic_var_in, "SOLVED");
}

///___________________________________________________________________________________________///
///___________________________________________________________________________________________///
///_____________________________________CLASS: SIMPLEX________________________________________///
///___________________________________________________________________________________________///
///___________________________________________________________________________________________///

//int st_w = 0;
int	simplex::check_w_valid() {
	//Assuming system is in canonical form
	int valid_basic_variables_counter = 0;
	for (auto& i : basic_var_in) {
		if (i < p.number_of_variables() - artificial_variables) ++valid_basic_variables_counter;
	}
	return valid_basic_variables_counter;
}

void simplex::add_artificial_variables() {

	////Add a number of variables equal to the number of constraints
	size_t var_numb = p.number_of_variables();
	for (size_t i = 1; i <= p.number_of_constraints(); ++i)
	{
		basic_var_in[i - 1] = var_numb + i - 1;
		p.push_var(i);
		++artificial_variables;
	}
	p.print_tableau();
	////Add w-problem
	std::vector<rational> w_problem(p.eq_size(), 0);
	for (size_t j = 0; j <= p.number_of_variables() - artificial_variables; ++j)
		for (size_t i = 1; i <= p.number_of_constraints(); ++i) {
			w_problem[j] = w_problem[j] - p.column(j)[i];
		}

	//Change objective function
	p.push_obj_function(w_problem);
	st_w = 1;
}
void simplex::remove_artificial_variables() {
	//Remove w-problem
	p.pop_obj_function();

	//Remove variables
	while (artificial_variables) {
		p.pop_var();
		--artificial_variables;
	}
	st_w = 0;
	st_fin = 0;
}

void simplex::make_constants_positive() {
	for (size_t i = 1; i <= p.number_of_constraints(); ++i)
		if (p.constant_terms()[i] < 0) p.mult_eq(i, -1);
}

void simplex::make_valid() {
	//CHECK IF STANDARD FORM HAS BEEN REACHED
	if (p.independent_term() == 0) {
		st_std = 1;
	}
	else {
		st_noadm = 1; st_std = 0;
		std::cout << "LP-Problem does not have any feasible solutions";
		exit(1);
	}

	std::vector<int> basic_map(p.number_of_variables(), 0);
	size_t valid_basic_variables = 0;
	for (size_t j = 1; j <= p.number_of_variables(); ++j) {
		if (is_canonical(p.column(j).vector())) {
			basic_map[j - 1] = 1;
			if (j < p.number_of_variables() - artificial_variables + 1) ++valid_basic_variables;
		}
	}

#ifndef NDEBUG
	std::cout << "Map of basic variables: " << std::endl;
	for (auto& o : basic_map) std::cout << o << " ";
	std::cout << std::endl;
#endif // !NDEBUG

	if (valid_basic_variables == p.number_of_constraints()) {
		std::cout << "W-PROBLEM SOLVED. STANDARD FORM FOUND" << std::endl;
	}
	else
	{

#ifndef NDEBUG
		std::cout << "W-PROBLEM SOLVED. STANDARD FORM NOT FOUND" << std::endl << "PROCEED TO REPLACE ARTIFICIAL VARIABLES WITH ORIGINALS" << std::endl;
#endif // !NDEBUG

		size_t missing_basic_variables = p.number_of_constraints() - valid_basic_variables;
		for (size_t i = 0; i < p.number_of_constraints(); ++i) {
			std::cout << p.number_of_variables() - artificial_variables + 1 << std::endl;
			if ((size_t)basic_var_in[i] >= p.number_of_variables() - artificial_variables + 1) {
				for (size_t j = 1; j <= p.number_of_variables(); ++j) {
					if (basic_map[j - 1] == 0 && p.equation(i)[j] != 0) {
						//For debugging
#ifndef NDEBUG
						std::cout << std::endl << "Pivot at: " << i << " " << j - 1 << std::endl << std::endl;
						std::cout << "Pivot at:" << i << " " << j << std::endl << std::endl;
#endif // NDEBUG
						p.piv(i+p.number_of_obj_functions(), j);
#ifndef NDEBUG
						p.print_tableau();

#endif // NDEBUG
						basic_var_in[i] = (int)j;
						basic_map[j - 1] = 1;

#ifndef NDEBUG
						std::cout << "Now, basic variables are: ";
						for (auto& o : basic_var_in) std::cout << o << " ";
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
			std::cout << "REMOVING REDUNDANT EQUATIONS" << std::endl;
#endif // !NDEBUG
			for (size_t i = 0; i < missing_basic_variables; ++i) {
				p.pop_eq();
#ifndef NDEBUG
				std::cout << "REMOVED " << i + 1 << (i == 0 ? " EQUATION" : "EQUATIONS") << std::endl;
#endif // !NDEBUG
			}
#ifndef NDEBUG
			std::cout << "PROBLEM BECOMES:" << std::endl;
			p.print_tableau();
#endif // !NDEBUG
		}
#ifndef NDEBUG
		std::cout << "W-PROBLEM SOLVED. STANDARD FORM HAS BEEN FOUND." << std::endl;
#endif // !NDEBUG
	}

}


void simplex::standard() {

	p.print_tableau();
	p.to_min_obj();
	p.print_tableau();
	p.add_slack_variables();
	p.print_tableau();
	make_constants_positive();
	p.print_tableau();
	add_artificial_variables();
	p.print_tableau();

	st_w = 1;
	st_fin = 0;

	//PERFORM OPERATIONS
	solution sol = simplex_base::solve(p, basic_var_in);
	p = sol.final_form();
	basic_var_in = sol.basic_variables();
	make_valid();
	//END OF OPERATIONS
	remove_artificial_variables();

	st_w = 0;
	st_fin = 0;
}
//
//std::vector<int>& simplex::basic_map();

solution simplex::solve(lp_problem p_out) {
	simplex problem(p_out);
	problem.standard();
	return simplex_base::solve(problem.p, problem.basic_var_in);
}

bool simplex::check_solvable(lp_problem p_out) {
	for (auto& it : p_out.boundaries())
		if (it <= 0) return 0;
	return 1;
}

///___________________________________________________________________________________________///
///___________________________________________________________________________________________///
///____________________________________CLASS: DUAL_SIMPLEX____________________________________///
///___________________________________________________________________________________________///
///___________________________________________________________________________________________///



///___________________________________________________________________________________________///
///___________________________________________________________________________________________///
///______________________________________CLASS: GOMORY________________________________________///
///___________________________________________________________________________________________///
///___________________________________________________________________________________________///


