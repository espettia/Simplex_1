#include "lp_problem.h"


lp_problem& lp_problem::push_eq(const std::vector<rational>& eq_out, int op_out) {
	
	m.push_r(eq_out);
	op.push_back(op_out);
	++constraints;
	return *this;

}
lp_problem& lp_problem::push_var(const std::vector<rational>& var_out, int bd_out) {
	
	m.push_c(var_out);
	bd.push_back(bd_out);
	++variables;
	return *this;

}

lp_problem& lp_problem::push_var(int n, rational val, int bd_out) {

	int index = n >= 0 ? n : constraints + 1 - n;
	std::vector<rational> temp(constraints + 1, 0);
	temp[index] = val;
	m.push_c(temp);
	bd.push_back(bd_out);
	++variables;
	return *this;

}
lp_problem& lp_problem::pop_eq() {

	m.pop_r();
	bd.pop_back();
	--constraints;
	return *this;
}
lp_problem& lp_problem::pop_var() {

	m.pop_c();
	op.pop_back();
	--variables;
	return *this;

}

lp_problem& lp_problem::piv(size_t i, size_t j) {
	m.piv(i, j);
	return *this;
}

lp_problem& lp_problem::mult_eq(int i, rational q) {

	m.mult_r(i, q);
	if (op[i] > 0) op[i] = -1;
	else if (op[i] < 0) op[i] = 1;
	return *this;
}

lp_problem& lp_problem::to_min_obj() {

	if (max_obj) {
		objective_function() = rational(-1) * objective_function().vector();
		max_obj = !max_obj;
	}
	return *this;
}

lp_problem& lp_problem::to_max_obj() {

	if (!max_obj) {
		objective_function() = rational(-1) * objective_function().vector();
		max_obj = !max_obj;
	}
	return *this;
}

lp_problem& lp_problem::to_min_problem() {

	to_min_obj();
	//Add or modify equations to obtain >= form
	for (size_t i = 1; i < constraints + 1; ++i) {
		if (op[i] == 0) {
			op.push_back(-1);
			m.push_r(m.row(i).vector());
			++constraints; //REVISAR, CREO QUE HABÍA ALGO ADICIONAL
		}
		else if (op[i] < 0)
			mult_eq(i, -1);
		op[i] = 1;
	}

	return *this;
}

lp_problem& lp_problem::to_max_problem() {

	to_max_obj();
	//Add or modify equations to obtain >= form
	for (size_t i = 1; i < constraints + 1; ++i) {
		if (op[i] == 0) {
			op.push_back(1);
			m.push_r(m.row(i).vector());
			++constraints;//LO MISMO, CREO QUE FALTA ALGO
		}
		else if (op[i] > 0) 
			mult_eq(i, -1);
		op[i] = 1;
	}
	return *this;

}

lp_problem& lp_problem::add_slack_variables() {

	for (size_t i = 1; i <= constraints; ++i) {
		if (op[i] > 0) //>=
			push_var(i, -1);
		else if (op[i] < 0) //<=
			push_var(i, 1);
		op[i] = 0;//=
	}
	return *this;

}

////Falta implementar esto
//std::vector<int> lp_problem::basic_var() {
//
//	std::vector<int> basic(m.columns(), -1);
//	for (size_t i = 1; i < constraints + 1; ++i) {
//		for (size_t j = 0; j < variables; ++j) {
//			if (m.at(i, j) == 1 && is_canonical(m.col(j).vector())) {
//				basic[i] = j; break;
//			}
//		}
//	}
//	return basic;
//
//}


//Esto también falta
std::vector<int> lp_problem::basic_map() {

	std::vector<int> basic(m.columns(), -1);
	for (size_t i = 0; i < constraints + 1; ++i) {
		for (size_t j = 0; j < variables; ++j) {
			if (m.at(i, j) == 1 && is_canonical(m.col(j).vector())) {
				basic[i] = j; break;
			}
		}
	}
	return basic;

}

lp_problem lp_problem::dual() {

	lp_problem p1 = *this;

	//p1.st_w = 0;

	p1.to_max_problem();

	p1.m = p1.m.transpose();

	p1.op = std::vector<int>(p1.m.rows() - 1, 1);

	size_t temp_constraints = p1.constraints;
	p1.constraints = p1.variables;
	p1.variables = temp_constraints;
	p1.max_obj = 0;
	p1.max_type = !p1.max_type;

	return p1;
}

void lp_problem::print_tableau() {
	for (auto& equation_it : equations_iterate())
	{
		for (auto& term : equation_it)
			std::cout << std::setw(10) << term;
		std::cout << std::endl;
	}
	for (auto& term : objective_function())
		std::cout << std::setw(10) << term;
	std::cout << std::endl << std::endl;
}

void lp_problem::print_op(size_t i) {
	if (op[i] < 0) std::cout << "<=";
	else if (op[i] > 0) std::cout << ">=";
	else std::cout << "=";
}

//Falta completar con los casos de bd
void lp_problem::print_problem() {

		size_t last = m.columns() - 1;

		//Print objective function
		std::cout << (max_obj ? "Maximize  " : "Minimize  ");
		int flag = 0;
		for (size_t c = 0; c < last; ++c) {
			if (objective_function()[c].sign() == 1 && flag == 1) std::cout << " +";
			objective_function()[c].print();
			if (objective_function()[c].sign() != 0) {
				std::cout << "X" << c << "  "; flag = 1;
			}
		}
		std::cout << " = ";
		independent_term().print(1);
		std::cout << (max_type == max_obj ? " + " : " - ") << f << " " <<std::endl;

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
		//add support for unrestricted variables


		std::cout << std::endl;
}

