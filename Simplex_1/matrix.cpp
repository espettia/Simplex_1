#include "matrix.h"


matrix::matrix(std::vector<std::vector<rational>> a) {
	m = a;
}

std::vector<rational> matrix::mult(rational q,  std::vector<rational> v) {
	std::vector<rational> ans = v;
	for (auto& i : ans)
		i = i * q;
	return ans;
}

size_t matrix::rows() {
	return m.size();
}

size_t matrix::columns() {
	if (m.empty())
		return 0;
	return m[0].size();
}

matrix matrix::sum_r(size_t r, std::vector<rational> v) {
	if (r < 0 || r >= v.size())
		exit(3);
	for (size_t i = 0; i < this->columns(); ++i)
	{
		if(i<v.size())
			m[r][i] = v[i] + m[r][i];
	}
	return *this;
}

matrix matrix::sum_c(size_t c, std::vector<rational> v) {
	if (c < 0 || c >= v.size())
		exit(3);
	for (size_t i = 0; i < this->rows(); ++i) {
		if (i < v.size())
			m[i][c] = v[i] + m[i][c];
	}
	return *this;
}

matrix matrix::piv(size_t r, size_t c) {
	if (r < 0 || r >= this->rows() || c < 0 || c >= this->columns())
		exit(3);

	for (size_t r_i = 0; r_i < this->rows(); ++r_i){
		if (r_i != r) {
			rational factor = (m[r_i][c] / m[r][c]) * -1;
			sum_r(r_i, mult(factor, m[r]));
		}
	}
	return *this;
}

void matrix::print() {
	for (size_t r_i = 0; r_i < this->rows(); ++r_i) {
		for (auto& c : m[r_i]) {
			std::cout << c.num() << "/" << c.den() << " ";
		}
		std::cout<<std::endl;
	}
	std::cout << std::endl;
}


matrix matrix::push_r(std::vector<rational> v) {
	if (v.size() != this->columns()) {
		exit(2);
	}
	m.push_back(v);
	return *this;
}

matrix matrix::push_c(std::vector<rational> v) {
	if (v.size() != this->rows()) {
		exit(2);
	}
	for (size_t r_i = 0; r_i < this->rows(); ++r_i) {
		size_t last_c = this->columns() - 1;
		m[r_i].push_back(v[r_i]);
	}
	return *this;
}

matrix matrix::pop_r() {
	if (!m.empty())
		m.pop_back();
	return *this;
}

matrix matrix::pop_c() {
	if (!m.empty())
		for (size_t r_i = 0; r_i < this->rows(); ++r_i) {
			size_t last_c = this->columns() - 1;
			m[r_i].pop_back();
		}
	return *this;
}
//
//matrix matrix::gauss() { ; }
//
//matrix matrix::trans() { ; }