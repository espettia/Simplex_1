#include "lp_problem.h"

lp_problem::lp_problem(matrix m_out, std::vector<int> op_out, int type_out, int integer_out, int init_out, int std, int fin, int nosol, int sol, int noadm, int w) {
	m = m_out;
	op = op_out;
	max_type = type_out;
	max_obj = max_type;
	integer = integer_out;
	st_init = init_out;
	init_col = m_out.columns();
	st_std = std;
	st_fin = fin;
	st_nosol = nosol;
	st_sol = sol;
	st_noadm = noadm;
	st_w = w;
}

lp_problem::lp_problem() {
	st_init = 0;
}


lp_problem lp_problem:: operator=(lp_problem p) {

	m = p.m;
	op = p.op;
	max_type = p.max_type;
	integer = p.integer;
	st_init = p.st_init;
	init_col = p.init_col;
	st_std = p.st_std;
	st_fin = p.st_fin;
	st_nosol = p.st_nosol;
	st_sol = p.st_sol;
	st_noadm = p.st_noadm;

	return *this;
}

std::vector<rational> lp_problem::objective_function() {
	if (!m.empty()) {
		return m.row(m.rows() - 1);
	}
	else
		exit(1);
}
//
//std::vector<rational> lp_problem::constraints() {
//	if (m.rows() > 0)
//	{
//
//	}
//}
std::vector<rational> lp_problem::constant_terms() {
	if (m.empty())
		exit(0);
	std::vector<rational> ans;
	size_t last = m.columns() - 1;
	for (size_t r_i = 0; r_i < m.rows(); ++r_i) {
		ans.push_back(m.at(r_i, last));
	}
	return ans;
}

std::vector<int> lp_problem::basic_var() {
	std::vector<int> ans (m.columns() - 1, 0);
	for (size_t j = 0; j < m.columns() - 1; ++j) {
		if (is_canonical(m.col(j))) ans[j]=1;
	}
	return ans;
}

bool lp_problem::is_canonical(std::vector<std::reference_wrapper<rational>> v) {
	int non_zero = 0;
	int one = 0;
	for (auto& c : v) {
	if (c.get() != 0) non_zero++;
	if (c.get() == 1) one++;
}
	return non_zero == 1 && one == 1;
}

bool lp_problem::init() {
	if (m.rows() < 2 || (op.size() != m.rows() - 1 && st_w == 0) || (op.size()!= m.rows()-2 && st_w==1) || m.columns() < 2) {
		std::cout << "Not valid parameters";
		st_init = false;
		exit(10);
	}
	st_init = true;
	return st_init;
}

bool lp_problem::check_dual_simplex() {
	for (auto& i : m.col(m.columns() - 1)) {
		if (i.get() < 0) {
			return 0;
		}
	}
	return 1;
}

bool lp_problem::check_solved() {
	for (size_t c = 0; c < m.row(m.rows() - 1).size() - 1; ++c) {
		if (m.row(m.rows() - 1)[c] < 0) return 0;
	}
	return 1;
}
bool lp_problem::check_unbounded() {
	for (size_t j = 0; j < m.columns() - 1; ++j) {
		if (m.at(m.rows() - 1, j) < 0) {
			int negative_counter = 0;
			for (size_t i = 0; i < m.rows() - 1; ++i) {
				if (m.at(i, j) <= 0) ++negative_counter;
			}
			if (negative_counter == m.rows() - 1) return 1;
		}
	}
	return 0;
}

int lp_problem::check_finished() {
	if (this->check_solved()) return st_sol = 1;
	if (this->check_unbounded()) return st_nosol = 1;
	return 0 ;
}

lp_problem lp_problem::to_min_problem() {

		if (!st_init) {
			std::cout << "Problem need to be initialized" << std::endl;
			exit(11);
		}

	if (max_obj) {
		for (size_t j = 0; j < m.columns(); ++j) {
			m.at(m.rows() - 1, j) = m.at(m.rows() - 1, j) * -1;
		}
		max_obj = 0;
	}

	std::vector<rational> temp_function = m.row(m.rows() - 1);
	m.pop_r();

	for (size_t i = 0; i < m.rows(); ++i) {

		if (op[i] == 1) {
			op[i] = 2;
			for (size_t j = 0; j < m.columns(); ++j) {
				m.at(i, j) = m.at(i, j) * -1;
			}
		}
		else if (op[i] == 0) {
			op[i] = 2;
			op.push_back(2);
			m.push_r(m.mult(rational(-1), m.row(i)));
		}
	}
	m.push_r(temp_function);
	return *this;
}

lp_problem lp_problem::add_slack_variables() {
	std::vector<rational> temp_constants = this->constant_terms();
	m.pop_c();

	for (size_t i = 0; i < m.rows()-1; ++i) {
		if (op[i] == 2) {
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = -1;
			m.push_c(slack);
		}
		else if (op[i] == 1) {
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = 1;
			m.push_c(slack);
		}
		op[i] = 0;
	}
	m.push_c(temp_constants);
	return *this;
}

lp_problem lp_problem::make_coefficients_positive() {
	std::vector<rational> constants = this->constant_terms();
	for (size_t i = 0; i < m.rows(); ++i) {
		if (constants[i] < 0) {
			m.row(i) = m.mult(-1, m.row(i));
		}
	}
	return *this;
}

lp_problem lp_problem::add_artificial_variables() {
	std::vector<rational> temp_constants = this->constant_terms();

	m.pop_c();
	for (size_t i = 0; i < m.rows() - 1; ++i) {
			std::vector<rational> slack(m.rows(), 0);
			slack[i] = 1;
			m.push_c(slack);
	}
	m.push_c(temp_constants);

	std::vector<rational> w_problem(m.columns(), 0);
	for (size_t i = 0; i < m.rows() - 1; ++i) {
		for (size_t j = 0; j < (m.columns() - 1) - (m.rows()-1); ++j) {
			w_problem[j] = w_problem[j] - m.at(i, j);
		}
		w_problem[m.columns()-1] = w_problem[m.columns() - 1] - m.at(i, m.columns() - 1);
	}
	m.push_r(w_problem);
	op.push_back(0);
	return *this;
}

lp_problem lp_problem::remove_artificial_variables() {
	std::vector<rational> temp_constants = this->constant_terms();
	m.pop_c();
	for (size_t i = 0; i < m.rows() - 2; ++i) {
		m.pop_c();
	}
	m.push_c(temp_constants);
	m.pop_r();
	op.pop_back();
	return *this;
}

lp_problem lp_problem::standard() {
	st_noadm = 0;
	if (!st_init) {
		std::cout << "Problem not initialized" << std::endl;
		exit(12);
	}
	this->to_min_problem();
	this->add_slack_variables();
	this->make_coefficients_positive();
	this->add_artificial_variables(); 
	st_std = 1;
	st_w = 1;
	st_fin = 0;
	this->solve();
	if (m.at(m.rows() - 1, m.columns() - 1) == 0) st_std = 1;
	else {
		st_noadm = 1; st_std = 0; st_fin = 1;
	}
	this->remove_artificial_variables();

	st_w = 0;
	st_sol = 0;
	st_nosol = 0;

	//int max_type = 0; //0 min, 1 max
	//int max_obj = 0; //0min ,1 max
	//int integer = 0; //0 real, 1 integer
	//int st_init = 0;//
	//size_t init_col = 0;
	//int st_std = 0;
	//int st_fin = 0;
	//int st_nosol = 0;
	//int st_sol = 0;
	//int st_noadm = 0;
	//int st_w = 0;

	return *this;
}

lp_problem lp_problem::solve() {
	rational ck_gr;
	rational ck;
	rational aik_lw;
	rational aik;
	size_t i_lw;
	size_t k_gr;
	int flag;

	if (!st_std) {
		std::cout << "LP-Problem need to be in standard form"; exit(6);
	}

	while (st_fin = this->check_finished(), !st_fin) {

		flag = 0;
		for (size_t k = 0; k < m.columns() - 1; ++k) {
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
		for (size_t i = 0; i < m.rows() - 1 - !!st_w; ++i) {
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

		std::cout << "pick:" << i_lw << " " << k_gr << std::endl;
		if (i_lw == 0 && k_gr == 2) 
			std::cout << "trol";
		m.piv(i_lw, k_gr);

		//For debugging
		this->print();
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
	if (!max_obj) {
		for (size_t j = 0; j < m.columns(); ++j) {
			m.at(m.rows() - 1, j) = m.at(m.rows() - 1, j) * -1;
		}
		max_obj = 1;
	}

	std::vector<rational> temp_function = m.row(m.rows() - 1);
	m.pop_r();

	for (size_t i = 0; i < m.rows(); ++i) {

		if (op[i] == 2) {
			op[i] = 1;
			for (size_t j = 0; j < m.columns(); ++j) {
				m.at(i, j) = m.at(i, j) * -1;
			}
		}
		else if (op[i] == 0) {
			op[i] = 1;
			op.push_back(1);
			m.push_r(m.mult(rational(-1), m.row(i)));
		}
	}
	m.push_r(temp_function);
	return *this;
}

lp_problem lp_problem::dual() {
	lp_problem p1 = *this;

	st_std = 0;
	st_fin = 0;
	st_nosol = 0;
	st_sol = 0;
	st_noadm = 0;
	st_w = 0;

	p1.to_max_problem();
	p1.m = p1.m.transpose();
	p1.max_obj = 0;
	p1.op = std::vector<int>(p1.m.rows() - 1, 2);

	st_std = 0;
	st_fin = 0;
	st_nosol = 0;
	st_sol = 0;
	st_noadm = 0;
	st_w = 0;

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
	std::vector<int> basic = basic_var();
	std::vector<rational> sol(m.rows() - 1);
	int flag = 0;
	for (size_t c = 0; c < m.rows() - 1; ++c) {
		if (flag == 1)
			std::cout << " ,";
		sol.push_back(m.at(c, m.columns() - 1) * basic[c]);
		(m.at(c, m.columns() - 1) * basic[c]).print(1);
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
		std::vector<rational> function = this->objective_function();
		std::cout << (max_obj? "Maximize  " : "Minimize  ");
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
		std::cout << (max_type == max_obj ? " +z " : " -z ");
		std::cout << std::endl;

		//Print constraints
		std::cout << "Subject to:" << std::endl;
		for (size_t i = 0; i < m.rows()-1; ++i) {
			flag = 0;
			for (size_t j = 0; j < last; ++j) {
				if (m.at(i, j).num() != 0) {
					if (m.at(i, j).sign() == 1 && flag == 1) {
						std::cout << " + ";
					}
					m.at(i, j).print();
					if (m.at(i, j).sign() != 0 ) std::cout << "X" << j << "  ";
					flag = 1;
				}
				else
					std::cout << "       ";
			}

			this->print_op(i);
			std::cout << "  ";
			m.at(i, last).print(1);
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	else
		std::cout << "Not initialized" << std::endl;
}

