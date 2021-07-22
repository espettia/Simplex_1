#pragma once
#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "rational.h"
#include "matrix.h"
#include "lp_problem.h"

class solution
{
private:
	lp_problem p_original;
	lp_problem p_final;
	std::vector<int> basic_var_in;
	std::string message;
public:
	solution(lp_problem a, lp_problem b): p_original(a), p_final(b) {}
	lp_problem final_form() { return p_final; }
	lp_problem original_form() { return p_original; }

};

class lp_algorithm
{
protected:

	lp_problem p_original;

	int st_std = 0;
	int st_fin = 0;
	int st_sol = 0;
	int st_noadm = 0;

public:

	lp_algorithm (lp_problem p_out): p_original(p_out){ }
	virtual bool check_solved() = 0;
	virtual bool check_unbounded() = 0;
	virtual bool check_finished() = 0;
	virtual std::vector<size_t> find_pivot() = 0;
};

class simplex_base : public lp_algorithm
{
protected:

	lp_problem p;
	std::vector<int> basic_var_in;

public:

	simplex_base(lp_problem p_out) : lp_algorithm(p_out) { p = p_original; basic_var_in = std::vector<int>(p_original.number_of_constraints(), -1); }
	//std::vector<int> basic_var_in;
	bool check_solved() override;
	bool check_unbounded() override;
	bool check_finished() override;

	std::vector<size_t> find_pivot() override;
	//std::vector<int>& basic_var();

	static bool check_solvable(lp_problem p_out);
	static solution solve(lp_problem p_out);

};

class simplex : public simplex_base
{
protected:
	lp_problem p_final;
	size_t artificial_variables = 0;
public:
	simplex(lp_problem p_out): simplex_base(p_out) { }
	int st_w = 0;
	int	check_w_valid();
	void add_artificial_variables();
	void remove_artificial_variables();
	void make_constants_positive();
	void make_valid();
	void standard();
	/*std::vector<int>& basic_map();*/
	static solution solve(lp_problem p_out);
	static bool check_solvable(lp_problem p_out);

};

class dual_simplex : public simplex_base
{

};

class gomory : public simplex
{

};
